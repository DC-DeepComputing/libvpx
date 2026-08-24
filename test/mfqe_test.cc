/*
 *  Copyright (c) 2026 The WebM project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#include <array>
#include <cstring>

#include "gtest/gtest.h"

// Declares vp9_mfqe().
#include "vp9/common/vp9_mfqe.h"
// Defines VP9_COMMON and related internal VP9 types.
#include "vp9/common/vp9_onyxc_int.h"
// Defines and allocates YV12 frame buffers.
#include "vpx_scale/yv12config.h"

namespace {

const int kFrameSize = 64;
const int kCheckerboardSquareSize = 4;

class TmpFrameBuffer {
 public:
  TmpFrameBuffer(int width, int height, int stride, bool fill_checkerboard) {
#if CONFIG_VP9_HIGHBITDEPTH
    allocation_status =
        vpx_alloc_frame_buffer(&buffer, stride, height, 1, 1, 0, 0, 0);
#else
    allocation_status =
        vpx_alloc_frame_buffer(&buffer, stride, height, 1, 1, 0, 0);
#endif
    if (allocation_status != 0) return;

    buffer.y_width = width;
    buffer.y_height = height;
    buffer.y_crop_width = width;
    buffer.y_crop_height = height;
    buffer.uv_width = width >> 1;
    buffer.uv_height = height >> 1;
    buffer.uv_crop_width = width >> 1;
    buffer.uv_crop_height = height >> 1;
    buffer.render_width = width;
    buffer.render_height = height;

    if (fill_checkerboard) {
      // Fill the luma plane with a checkerboard pattern.
      for (int row = 0; row < height; ++row) {
        for (int col = 0; col < width; ++col) {
          const int checkerboard_row = row / kCheckerboardSquareSize;
          const int checkerboard_col = col / kCheckerboardSquareSize;
          int pixel_value = 0x20;
          if ((checkerboard_row + checkerboard_col) % 2 != 0) {
            pixel_value = 0xe0;
          }
          buffer.y_buffer[row * buffer.y_stride + col] = pixel_value;
        }
      }
    } else {
      // Use a distinct initial luma value so copying is observable.
      for (int row = 0; row < height; ++row) {
        memset(buffer.y_buffer + row * buffer.y_stride, 0, width);
      }
    }

    // Use neutral chroma so the checkerboard pattern is luma-only.
    for (int row = 0; row < height / 2; ++row) {
      memset(buffer.u_buffer + row * buffer.uv_stride, 0x80, width / 2);
      memset(buffer.v_buffer + row * buffer.uv_stride, 0x80, width / 2);
    }
  }
  ~TmpFrameBuffer() { vpx_free_frame_buffer(&buffer); }

  int get_allocation_status() const { return allocation_status; }

  YV12_BUFFER_CONFIG *get() { return &buffer; }

 private:
  YV12_BUFFER_CONFIG buffer = {};
  int allocation_status = -1;
};

TEST(MfqeTest, Copy64x64WithDifferentStrides) {
  TmpFrameBuffer source(kFrameSize, kFrameSize, kFrameSize,
                        /*fill_checkerboard=*/true);
  // The wider allocation produces a different stride while preserving the
  // visible dimensions of the image.
  TmpFrameBuffer destination(kFrameSize, kFrameSize, kFrameSize + 16,
                             /*fill_checkerboard=*/false);
  ASSERT_EQ(0, source.get_allocation_status());
  ASSERT_EQ(0, destination.get_allocation_status());
  ASSERT_NE(source.get()->y_stride, destination.get()->y_stride);

  // One MODE_INFO entry per 8x8 MI unit in this 64x64 superblock.
  std::array<MODE_INFO, MI_BLOCK_SIZE * MI_BLOCK_SIZE> mi = {};
  mi[0].sb_type = BLOCK_64X64;
  mi[0].mode = DC_PRED;  // Force the copy path rather than MFQE filtering.

  VP9_COMMON cm = {};
  cm.frame_type = INTER_FRAME;
  cm.frame_to_show = source.get();
  cm.post_proc_buffer = *destination.get();
  cm.mi_rows = MI_BLOCK_SIZE;
  cm.mi_cols = MI_BLOCK_SIZE;
  cm.mi_stride = MI_BLOCK_SIZE;
  cm.mi = mi.data();

  vp9_mfqe(&cm);

  for (int row = 0; row < kFrameSize; ++row) {
    EXPECT_EQ(0,
              std::memcmp(source.get()->y_buffer + row * source.get()->y_stride,
                          cm.post_proc_buffer.y_buffer +
                              row * cm.post_proc_buffer.y_stride,
                          kFrameSize))
        << "row " << row;
  }
}

}  // namespace
