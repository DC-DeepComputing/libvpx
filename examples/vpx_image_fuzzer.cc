/*
 *  Copyright (c) 2026 The WebM project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

/*
 * Fuzzer for the libvpx image API
 * ===============================
 * Requirements
 * --------------
 * Requires Clang 6.0 or above as -fsanitize=fuzzer is used as a linker
 * option.
 *
 * Steps to build
 * --------------
 * Clone libvpx repository
   $git clone https://chromium.googlesource.com/webm/libvpx

 * Create a directory in parallel to libvpx and change directory
   $mkdir vpx_image_fuzzer
   $cd vpx_image_fuzzer/

 * Enable sanitizers (Supported: address integer memory thread undefined)
   $source ../libvpx/tools/set_analyzer_env.sh address

 * Configure libvpx.
   $../libvpx/configure --disable-unit-tests --disable-examples \
   --enable-debug --extra-cflags="-fsanitize=fuzzer-no-link"

 * Build libvpx
   $make -j32

 * Build the image fuzzer
   $ $CXX $CXXFLAGS -std=gnu++17 -Wall -Wextra -Wimplicit-fallthrough \
   -fsanitize=fuzzer -I../libvpx -I. -Wl,--start-group \
   ../libvpx/examples/vpx_image_fuzzer.cc -o ./vpx_image_fuzzer \
   ./libvpx.a -Wl,--end-group

 * The fuzzer drives the image API directly and decodes no bitstream, so an
 * empty corpus directory is sufficient
   $mkdir CORPUS

 * Run fuzzing:
   $./vpx_image_fuzzer CORPUS

 * References:
 * http://llvm.org/docs/LibFuzzer.html
 * https://github.com/google/oss-fuzz
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include <limits>

#include "vpx/vpx_image.h"

namespace {

constexpr unsigned int kMaxDimension = 64;
constexpr unsigned int kMaxOperations = 64;

struct FuzzReader {
  const uint8_t *data;
  size_t size;

  uint8_t ReadU8() {
    if (size == 0) return 0;
    const uint8_t value = *data++;
    --size;
    return value;
  }
};

vpx_img_fmt_t PickFormat(uint8_t value) {
  static constexpr vpx_img_fmt_t kFormats[] = {
    VPX_IMG_FMT_YV12,   VPX_IMG_FMT_I420,   VPX_IMG_FMT_I422,
    VPX_IMG_FMT_I444,   VPX_IMG_FMT_I440,   VPX_IMG_FMT_NV12,
    VPX_IMG_FMT_I42016, VPX_IMG_FMT_I42216, VPX_IMG_FMT_I44416,
    VPX_IMG_FMT_I44016,
  };
  return kFormats[value % (sizeof(kFormats) / sizeof(kFormats[0]))];
}

unsigned int PickDimension(uint8_t value) {
  return value == UINT8_MAX ? 0 : 1u + value % kMaxDimension;
}

unsigned int PickWithin(uint8_t value, unsigned int limit) {
  return value % (limit + 1);
}

struct BufferBounds {
  uintptr_t begin;
  uintptr_t end;
};

BufferBounds GetBufferBounds(const vpx_image_t *img) {
  const size_t y_size = static_cast<size_t>(img->h) *
                        static_cast<size_t>(img->stride[VPX_PLANE_Y]);
  const unsigned int chroma_height = img->h >> img->y_chroma_shift;
  size_t size = y_size;
  if (img->fmt == VPX_IMG_FMT_NV12) {
    size += static_cast<size_t>(chroma_height) *
            static_cast<size_t>(img->stride[VPX_PLANE_U]);
  } else {
    size += static_cast<size_t>(chroma_height) *
            (static_cast<size_t>(img->stride[VPX_PLANE_U]) +
             static_cast<size_t>(img->stride[VPX_PLANE_V]));
  }
  const uintptr_t begin = reinterpret_cast<uintptr_t>(img->img_data);
  return { begin, begin + size };
}

void CheckPlanePointers(const vpx_image_t *img, const BufferBounds &bounds) {
  for (int plane = VPX_PLANE_Y; plane <= VPX_PLANE_V; ++plane) {
    if (img->planes[plane] == nullptr) continue;
    const uintptr_t address = reinterpret_cast<uintptr_t>(img->planes[plane]);
    if (address < bounds.begin || address > bounds.end) abort();
  }
}

unsigned int PlaneWidth(const vpx_image_t *img, int plane) {
  if (plane == VPX_PLANE_Y) return img->d_w;
  return (img->d_w + img->x_chroma_shift) >> img->x_chroma_shift;
}

unsigned int PlaneHeight(const vpx_image_t *img, int plane) {
  if (plane == VPX_PLANE_Y) return img->d_h;
  return (img->d_h + img->y_chroma_shift) >> img->y_chroma_shift;
}

void TouchImage(vpx_image_t *img) {
  if (img->d_w == 0 || img->d_h == 0) return;
  const size_t bytes_per_sample = (img->fmt & VPX_IMG_FMT_HIGHBITDEPTH) ? 2 : 1;

  for (int plane = VPX_PLANE_Y; plane <= VPX_PLANE_V; ++plane) {
    if (img->fmt == VPX_IMG_FMT_NV12 && plane == VPX_PLANE_V) continue;
    if (img->planes[plane] == nullptr) continue;

    size_t row_bytes =
        static_cast<size_t>(PlaneWidth(img, plane)) * bytes_per_sample;
    if (img->fmt == VPX_IMG_FMT_NV12 && plane == VPX_PLANE_U) {
      row_bytes *= 2;
    }
    const unsigned int height = PlaneHeight(img, plane);
    if (row_bytes == 0 || height == 0) continue;

    const unsigned int rows[] = { 0, height - 1 };
    for (const unsigned int row_index : rows) {
      volatile uint8_t *row =
          img->planes[plane] +
          static_cast<ptrdiff_t>(row_index) * img->stride[plane];
      row[0] = 0;
      row[row_bytes - 1] = 0;
    }
  }
}

void SetRandomRect(vpx_image_t *img, FuzzReader *reader) {
  const unsigned int x = PickWithin(reader->ReadU8(), img->w);
  const unsigned int y = PickWithin(reader->ReadU8(), img->h);
  const unsigned int w = PickWithin(reader->ReadU8(), img->w - x);
  const unsigned int h = PickWithin(reader->ReadU8(), img->h - y);
  if (vpx_img_set_rect(img, x, y, w, h) != 0) abort();
}

void SetEmptyHeight(vpx_image_t *img, FuzzReader *reader) {
  const unsigned int x = PickWithin(reader->ReadU8(), img->w);
  const unsigned int y = PickWithin(reader->ReadU8(), img->h);
  const unsigned int w = PickWithin(reader->ReadU8(), img->w - x);
  if (vpx_img_set_rect(img, x, y, w, 0) != 0) abort();
}

void SetEmptyWidth(vpx_image_t *img, FuzzReader *reader) {
  const unsigned int x = PickWithin(reader->ReadU8(), img->w);
  const unsigned int y = PickWithin(reader->ReadU8(), img->h);
  const unsigned int h = PickWithin(reader->ReadU8(), img->h - y);
  if (vpx_img_set_rect(img, x, y, 0, h) != 0) abort();
}

}  // namespace

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
  FuzzReader reader = { data, size };
  const vpx_img_fmt_t format = PickFormat(reader.ReadU8());
  const unsigned int width = PickDimension(reader.ReadU8());
  const unsigned int height = PickDimension(reader.ReadU8());
  const unsigned int align = 1u << (reader.ReadU8() % 7);

  vpx_image_t *img = vpx_img_alloc(nullptr, format, width, height, align);
  if (img == nullptr) return 0;

  const BufferBounds bounds = GetBufferBounds(img);
  CheckPlanePointers(img, bounds);
  TouchImage(img);

  for (unsigned int operation = 0;
       operation < kMaxOperations && reader.size > 0; ++operation) {
    switch (reader.ReadU8() % 8) {
      case 0: vpx_img_flip(img); break;
      case 1: SetRandomRect(img, &reader); break;
      case 2: SetEmptyHeight(img, &reader); break;
      case 3:
        if (vpx_img_set_rect(img, 0, 0, img->w, img->h) != 0) abort();
        break;
      case 4: SetEmptyWidth(img, &reader); break;
      case 5:
        vpx_img_flip(img);
        vpx_img_flip(img);
        break;
      case 6:
        if (vpx_img_set_rect(img, std::numeric_limits<unsigned int>::max(),
                             std::numeric_limits<unsigned int>::max(), 1,
                             1) == 0) {
          abort();
        }
        break;
      default: break;
    }
    CheckPlanePointers(img, bounds);
    TouchImage(img);
  }

  vpx_img_free(img);
  return 0;
}
