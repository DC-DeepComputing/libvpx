/*
 *  Copyright (c) 2014 The WebM project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#include <array>
#include <vector>

#include "gtest/gtest.h"

#include "./vpx_config.h"
#if CONFIG_VP8_DECODER && CONFIG_MULTITHREAD && defined(__linux__)
#include <stdio.h>
#include <sys/resource.h>
#endif
#include "test/ivf_video_source.h"
#include "test/video_source.h"
#if CONFIG_VP8_ENCODER || CONFIG_VP9_ENCODER
#include "vpx/vp8cx.h"
#endif
#include "vpx/vp8dx.h"
#include "vpx/vpx_decoder.h"
#include "vpx/vpx_encoder.h"

namespace {

#define NELEMENTS(x) static_cast<int>(sizeof(x) / sizeof(x[0]))

TEST(DecodeAPI, InvalidParams) {
  static vpx_codec_iface_t *kCodecs[] = {
#if CONFIG_VP8_DECODER
    &vpx_codec_vp8_dx_algo,
#endif
#if CONFIG_VP9_DECODER
    &vpx_codec_vp9_dx_algo,
#endif
  };
  uint8_t buf[1] = { 0 };
  vpx_codec_ctx_t dec;

  EXPECT_EQ(vpx_codec_dec_init(nullptr, nullptr, nullptr, 0),
            VPX_CODEC_INVALID_PARAM);
  EXPECT_EQ(vpx_codec_dec_init(&dec, nullptr, nullptr, 0),
            VPX_CODEC_INVALID_PARAM);
  EXPECT_EQ(vpx_codec_decode(nullptr, nullptr, 0, nullptr, 0),
            VPX_CODEC_INVALID_PARAM);
  EXPECT_EQ(vpx_codec_decode(nullptr, buf, 0, nullptr, 0),
            VPX_CODEC_INVALID_PARAM);
  EXPECT_EQ(vpx_codec_decode(nullptr, buf, NELEMENTS(buf), nullptr, 0),
            VPX_CODEC_INVALID_PARAM);
  EXPECT_EQ(vpx_codec_decode(nullptr, nullptr, NELEMENTS(buf), nullptr, 0),
            VPX_CODEC_INVALID_PARAM);
  EXPECT_EQ(vpx_codec_destroy(nullptr), VPX_CODEC_INVALID_PARAM);
  EXPECT_NE(vpx_codec_error(nullptr), nullptr);
  EXPECT_EQ(vpx_codec_error_detail(nullptr), nullptr);

  for (int i = 0; i < NELEMENTS(kCodecs); ++i) {
    EXPECT_EQ(VPX_CODEC_INVALID_PARAM,
              vpx_codec_dec_init(nullptr, kCodecs[i], nullptr, 0));

    EXPECT_EQ(VPX_CODEC_OK, vpx_codec_dec_init(&dec, kCodecs[i], nullptr, 0));
    EXPECT_EQ(VPX_CODEC_UNSUP_BITSTREAM,
              vpx_codec_decode(&dec, buf, NELEMENTS(buf), nullptr, 0));
    EXPECT_EQ(VPX_CODEC_INVALID_PARAM,
              vpx_codec_decode(&dec, nullptr, NELEMENTS(buf), nullptr, 0));
    EXPECT_EQ(VPX_CODEC_INVALID_PARAM,
              vpx_codec_decode(&dec, buf, 0, nullptr, 0));

    EXPECT_EQ(VPX_CODEC_OK, vpx_codec_destroy(&dec));
  }
}

#if CONFIG_VP8_DECODER
TEST(DecodeAPI, OptionalParams) {
  vpx_codec_ctx_t dec;

#if CONFIG_ERROR_CONCEALMENT
  EXPECT_EQ(VPX_CODEC_OK,
            vpx_codec_dec_init(&dec, &vpx_codec_vp8_dx_algo, nullptr,
                               VPX_CODEC_USE_ERROR_CONCEALMENT));
#else
  EXPECT_EQ(VPX_CODEC_INCAPABLE,
            vpx_codec_dec_init(&dec, &vpx_codec_vp8_dx_algo, nullptr,
                               VPX_CODEC_USE_ERROR_CONCEALMENT));
#endif  // CONFIG_ERROR_CONCEALMENT
}

TEST(DecodeAPI, Vp8FlushAfterFragmentError) {
  static constexpr uint8_t kInvalidKeyframe[10] = { 0 };
  vpx_codec_ctx_t dec;

  ASSERT_EQ(VPX_CODEC_OK,
            vpx_codec_dec_init(&dec, &vpx_codec_vp8_dx_algo, nullptr,
                               VPX_CODEC_USE_INPUT_FRAGMENTS));
  EXPECT_EQ(VPX_CODEC_OK,
            vpx_codec_decode(&dec, kInvalidKeyframe, sizeof(kInvalidKeyframe),
                             nullptr, 0));
  EXPECT_EQ(VPX_CODEC_UNSUP_BITSTREAM,
            vpx_codec_decode(&dec, nullptr, 0, nullptr, 0));
  EXPECT_EQ(VPX_CODEC_OK, vpx_codec_decode(&dec, nullptr, 0, nullptr, 0));
  EXPECT_EQ(VPX_CODEC_OK, vpx_codec_destroy(&dec));
}

#if CONFIG_SIZE_LIMIT && \
    (DECODE_WIDTH_LIMIT < 16383 || DECODE_HEIGHT_LIMIT < 16383)
TEST(DecodeAPI, Vp8RejectsOversizedKeyframeBeforeInitialization) {
  vpx_codec_ctx_t dec;
  ASSERT_EQ(VPX_CODEC_OK,
            vpx_codec_dec_init(&dec, &vpx_codec_vp8_dx_algo, nullptr, 0));

#if DECODE_WIDTH_LIMIT < 16383
  constexpr uint16_t kWidth = DECODE_WIDTH_LIMIT + 1;
  constexpr uint16_t kHeight = 1;
#else
  constexpr uint16_t kWidth = 1;
  constexpr uint16_t kHeight = DECODE_HEIGHT_LIMIT + 1;
#endif
  static constexpr uint8_t kOversizedKeyframe[10] = {
    0x00,
    0x00,
    0x00,
    0x9d,
    0x01,
    0x2a,
    static_cast<uint8_t>(kWidth),
    static_cast<uint8_t>(kWidth >> 8),
    static_cast<uint8_t>(kHeight),
    static_cast<uint8_t>(kHeight >> 8),
  };
  EXPECT_EQ(
      VPX_CODEC_CORRUPT_FRAME,
      vpx_codec_decode(&dec, kOversizedKeyframe,
                       static_cast<unsigned int>(sizeof(kOversizedKeyframe)),
                       nullptr, 0));

  static constexpr uint8_t kInterframe[10] = { 0x01 };
  EXPECT_EQ(VPX_CODEC_UNSUP_BITSTREAM,
            vpx_codec_decode(&dec, kInterframe,
                             static_cast<unsigned int>(sizeof(kInterframe)),
                             nullptr, 0));
  EXPECT_EQ(VPX_CODEC_OK, vpx_codec_destroy(&dec));
}
#endif  // CONFIG_SIZE_LIMIT && dimension limit is representable in VP8

TEST(DecodeAPI, Vp8FlushWithNoFragments) {
  vpx_codec_ctx_t dec;
  vpx_codec_dec_cfg_t cfg = { 1, 0, 0 };
  vpx_codec_flags_t flags = VPX_CODEC_USE_INPUT_FRAGMENTS;

  EXPECT_EQ(VPX_CODEC_OK,
            vpx_codec_dec_init(&dec, &vpx_codec_vp8_dx_algo, &cfg, flags));
  EXPECT_EQ(VPX_CODEC_OK, vpx_codec_decode(&dec, nullptr, 0, nullptr, 0));
  EXPECT_EQ(VPX_CODEC_OK, vpx_codec_destroy(&dec));
}

#if CONFIG_VP8_ENCODER && CONFIG_MULTITHREAD
// Encodes a sequence of key frames using two token partitions, truncates the
// second token partition of one frame so that the worker thread's bool decoder
// runs past the end of its partition, and then verifies that the
// multi-threaded decoder still reports subsequent well-formed key frames as
// not corrupted.
TEST(DecodeAPI, Vp8MultiThreadedCorruptedStateResetAcrossFrames) {
  constexpr int kWidth = 16;
  constexpr int kHeight = 32;
  constexpr int kFrames = 4;

  std::vector<std::vector<uint8_t>> frames;
  {
    vpx_codec_ctx_t enc;
    vpx_codec_enc_cfg_t cfg;
    ASSERT_EQ(vpx_codec_enc_config_default(&vpx_codec_vp8_cx_algo, &cfg, 0),
              VPX_CODEC_OK);
    cfg.g_w = kWidth;
    cfg.g_h = kHeight;
    cfg.g_lag_in_frames = 0;
    ASSERT_EQ(vpx_codec_enc_init(&enc, &vpx_codec_vp8_cx_algo, &cfg, 0),
              VPX_CODEC_OK);
    ASSERT_EQ(vpx_codec_control(&enc, VP8E_SET_TOKEN_PARTITIONS,
                                VP8_TWO_TOKENPARTITION),
              VPX_CODEC_OK);

    libvpx_test::RandomVideoSource video;
    video.SetSize(kWidth, kHeight);
    video.set_limit(kFrames);
    for (video.Begin(); video.img() != nullptr; video.Next()) {
      ASSERT_EQ(
          vpx_codec_encode(&enc, video.img(), video.pts(), video.duration(),
                           VPX_EFLAG_FORCE_KF, VPX_DL_REALTIME),
          VPX_CODEC_OK);
      vpx_codec_iter_t iter = nullptr;
      const vpx_codec_cx_pkt_t *pkt;
      while ((pkt = vpx_codec_get_cx_data(&enc, &iter)) != nullptr) {
        if (pkt->kind != VPX_CODEC_CX_FRAME_PKT) continue;
        ASSERT_NE(pkt->data.frame.flags & VPX_FRAME_IS_KEY, 0u);
        const uint8_t *buf = static_cast<const uint8_t *>(pkt->data.frame.buf);
        frames.emplace_back(buf, buf + pkt->data.frame.sz);
      }
    }
    ASSERT_EQ(vpx_codec_destroy(&enc), VPX_CODEC_OK);
  }
  ASSERT_EQ(frames.size(), static_cast<size_t>(kFrames));

  // Truncate frame 1 so that only a single byte of the second token partition
  // remains. The first token partition (used by the main thread) is left
  // intact.
  {
    std::vector<uint8_t> &f = frames[1];
    ASSERT_GT(f.size(), 10u);
    const size_t first_part_sz =
        (static_cast<size_t>(f[0]) | (static_cast<size_t>(f[1]) << 8) |
         (static_cast<size_t>(f[2]) << 16)) >>
        5;
    const size_t part_sizes = 10 + first_part_sz;
    ASSERT_LT(part_sizes + 3, f.size());
    const size_t part0_sz = static_cast<size_t>(f[part_sizes]) |
                            (static_cast<size_t>(f[part_sizes + 1]) << 8) |
                            (static_cast<size_t>(f[part_sizes + 2]) << 16);
    const size_t part1_start = part_sizes + 3 + part0_sz;
    ASSERT_LT(part1_start, f.size());
    f.resize(part1_start + 1);
  }

  vpx_codec_ctx_t dec;
  vpx_codec_dec_cfg_t dec_cfg = { /*threads=*/4, /*w=*/0, /*h=*/0 };
  ASSERT_EQ(vpx_codec_dec_init(&dec, &vpx_codec_vp8_dx_algo, &dec_cfg, 0),
            VPX_CODEC_OK);
  for (int i = 0; i < kFrames; ++i) {
    const vpx_codec_err_t res = vpx_codec_decode(
        &dec, frames[i].data(), static_cast<unsigned int>(frames[i].size()),
        /*user_priv=*/nullptr, /*deadline=*/0);
    vpx_codec_iter_t iter = nullptr;
    while (vpx_codec_get_frame(&dec, &iter)) {
    }
    if (i == 1) continue;
    EXPECT_EQ(res, VPX_CODEC_OK)
        << "frame " << i << ": " << vpx_codec_error_detail(&dec);
    int corrupted = -1;
    EXPECT_EQ(vpx_codec_control(&dec, VP8D_GET_FRAME_CORRUPTED, &corrupted),
              VPX_CODEC_OK);
    EXPECT_EQ(corrupted, 0) << "frame " << i;
  }
  EXPECT_EQ(vpx_codec_destroy(&dec), VPX_CODEC_OK);
}

// Issue: 559707641
// Encodes a sequence of frames with two token partitions, truncates the second
// token partition of one frame (used by a worker thread), and verifies that
// the multi-threaded decoder returns VPX_CODEC_CORRUPT_FRAME and does not emit
// a partially written frame. Also verifies that subsequent well-formed frames
// still decode correctly.
TEST(DecodeAPI, Vp8MultiThreadedWorkerDecodeErrorFailsFrame) {
  constexpr int kWidth = 32;
  constexpr int kHeight = 32;
  class PatternVideoSource : public libvpx_test::DummyVideoSource {
   protected:
    void FillFrame() override {
      if (!img_) return;
      // Keep row 0 flat so partition 0 size is small.
      memset(img_->planes[VPX_PLANE_Y], 128, img_->stride[VPX_PLANE_Y] * 16);
      // Fill row 1 with varying pattern so partition 1 has tokens to truncate.
      for (int y = 16; y < 32; ++y) {
        uint8_t *row =
            img_->planes[VPX_PLANE_Y] + y * img_->stride[VPX_PLANE_Y];
        for (int x = 0; x < 32; ++x) {
          row[x] = static_cast<uint8_t>((x * 37 + y * 73) ^ (frame_ * 19));
        }
      }
      memset(img_->planes[VPX_PLANE_U], 128, img_->stride[VPX_PLANE_U] * 16);
      memset(img_->planes[VPX_PLANE_V], 128, img_->stride[VPX_PLANE_V] * 16);
    }
  };

  constexpr int kFrames = 4;
  std::vector<std::vector<uint8_t>> frames;
  {
    vpx_codec_ctx_t enc;
    vpx_codec_enc_cfg_t cfg;
    ASSERT_EQ(vpx_codec_enc_config_default(&vpx_codec_vp8_cx_algo, &cfg, 0),
              VPX_CODEC_OK);
    cfg.g_w = kWidth;
    cfg.g_h = kHeight;
    cfg.g_lag_in_frames = 0;
    ASSERT_EQ(vpx_codec_enc_init(&enc, &vpx_codec_vp8_cx_algo, &cfg, 0),
              VPX_CODEC_OK);
    ASSERT_EQ(vpx_codec_control(&enc, VP8E_SET_TOKEN_PARTITIONS,
                                VP8_TWO_TOKENPARTITION),
              VPX_CODEC_OK);

    PatternVideoSource video;
    video.SetSize(kWidth, kHeight);
    video.set_limit(kFrames);
    for (video.Begin(); video.img() != nullptr; video.Next()) {
      ASSERT_EQ(
          vpx_codec_encode(&enc, video.img(), video.pts(), video.duration(),
                           VPX_EFLAG_FORCE_KF, VPX_DL_REALTIME),
          VPX_CODEC_OK);
      vpx_codec_iter_t iter = nullptr;
      const vpx_codec_cx_pkt_t *pkt;
      while ((pkt = vpx_codec_get_cx_data(&enc, &iter)) != nullptr) {
        if (pkt->kind != VPX_CODEC_CX_FRAME_PKT) continue;
        ASSERT_NE(pkt->data.frame.flags & VPX_FRAME_IS_KEY, 0u);
        const uint8_t *buf = static_cast<const uint8_t *>(pkt->data.frame.buf);
        frames.emplace_back(buf, buf + pkt->data.frame.sz);
      }
    }
    ASSERT_EQ(vpx_codec_destroy(&enc), VPX_CODEC_OK);
  }
  ASSERT_EQ(frames.size(), static_cast<size_t>(kFrames));

  // Truncate frame 1 so that only a single byte of the second token partition
  // remains. The first token partition (used by the main thread) is left
  // intact.
  {
    std::vector<uint8_t> &f = frames[1];
    ASSERT_GT(f.size(), 10u);
    const size_t first_part_sz =
        (static_cast<size_t>(f[0]) | (static_cast<size_t>(f[1]) << 8) |
         (static_cast<size_t>(f[2]) << 16)) >>
        5;
    const size_t part_sizes = 10 + first_part_sz;
    ASSERT_LT(part_sizes + 3, f.size());
    const size_t part0_sz = static_cast<size_t>(f[part_sizes]) |
                            (static_cast<size_t>(f[part_sizes + 1]) << 8) |
                            (static_cast<size_t>(f[part_sizes + 2]) << 16);
    const size_t part1_start = part_sizes + 3 + part0_sz;
    ASSERT_LT(part1_start, f.size());
    f.resize(part1_start + 1);
  }

  vpx_codec_ctx_t dec;
  vpx_codec_dec_cfg_t dec_cfg = { /*threads=*/4, /*w=*/0, /*h=*/0 };
  ASSERT_EQ(vpx_codec_dec_init(&dec, &vpx_codec_vp8_dx_algo, &dec_cfg, 0),
            VPX_CODEC_OK);
  for (int i = 0; i < kFrames; ++i) {
    const vpx_codec_err_t res = vpx_codec_decode(
        &dec, frames[i].data(), static_cast<unsigned int>(frames[i].size()),
        /*user_priv=*/nullptr, /*deadline=*/0);
    vpx_codec_iter_t iter = nullptr;
    const vpx_image_t *img = vpx_codec_get_frame(&dec, &iter);
    if (i == 1) {
      EXPECT_EQ(res, VPX_CODEC_CORRUPT_FRAME);
      EXPECT_EQ(img, nullptr);
      continue;
    }
    EXPECT_EQ(res, VPX_CODEC_OK)
        << "frame " << i << ": " << vpx_codec_error_detail(&dec);
    EXPECT_NE(img, nullptr) << "frame " << i;
    int corrupted = -1;
    EXPECT_EQ(vpx_codec_control(&dec, VP8D_GET_FRAME_CORRUPTED, &corrupted),
              VPX_CODEC_OK);
    EXPECT_EQ(corrupted, 0) << "frame " << i;
  }
  EXPECT_EQ(vpx_codec_destroy(&dec), VPX_CODEC_OK);
}
#endif  // CONFIG_VP8_ENCODER && CONFIG_MULTITHREAD

#if CONFIG_MULTITHREAD && defined(__linux__)
std::vector<uint8_t> MakeVp8KeyFrame(int w, int h, int header_w, int header_h,
                                     bool two_partitions, bool truncate_part0) {
  const int mbw = (w + 15) >> 4;
  const int mbs = mbw * ((h + 15) >> 4);
  uint32_t low = 0;
  uint32_t range = 255;
  int count = -24;
  std::vector<uint8_t> p1;
  auto put_bit = [&](int bit) {
    const uint32_t split = 1 + (((range - 1) * 128) >> 8);
    uint32_t r = split;
    if (bit) {
      low += split;
      r = range - split;
    }
    int shift = 0;
    while ((r << shift) < 128) ++shift;
    r <<= shift;
    int c = count + shift;
    if (c >= 0) {
      const int off = shift - c;
      if ((low << (off - 1)) & 0x80000000u) {
        size_t x = p1.size() - 1;
        while (x > 0 && p1[x] == 255) p1[x--] = 0;
        p1[x]++;
      }
      p1.push_back(static_cast<uint8_t>((low >> (24 - off)) & 255));
      low = (low << off) & 0xffffffu;
      shift = c;
      c -= 8;
    }
    low <<= shift;
    count = c;
    range = r;
  };
  auto put_bits = [&](int v, int n) {
    for (int i = n - 1; i >= 0; --i) put_bit((v >> i) & 1);
  };
  put_bits(0, 1);                       // color_space
  put_bits(0, 1);                       // clamping_type
  put_bits(0, 1);                       // segmentation_enabled
  put_bits(0, 1);                       // filter_type
  put_bits(63, 6);                      // loop_filter_level
  put_bits(0, 3);                       // sharpness_level
  put_bits(0, 1);                       // mode_ref_lf_delta_enabled
  put_bits(two_partitions ? 1 : 0, 2);  // log2_nbr_of_dct_partitions
  put_bits(20, 7);                      // y_ac_qi
  put_bits(0, 5);                       // delta_q flags
  put_bits(0, 1);                       // refresh_entropy_probs
  for (int i = 0; i < 32; ++i) put_bit(0);

  const size_t p1len = p1.size() + (mbs * 3 + 3) / 4 + 64;
  const size_t np = two_partitions ? 2 : 1;
  const size_t room = (mbs * 4 + np - 1) / np + mbw * 4 + 64;
  const size_t len0 = two_partitions ? (truncate_part0 ? 1 : room) : room;
  const size_t total_len = two_partitions ? (len0 + room) : room;
  std::vector<uint8_t> out(10 + p1len + 3 * (np - 1) + total_len, 0);
  const uint32_t tag = (1u << 4) | (static_cast<uint32_t>(p1len) << 5);
  out[0] = tag & 255;
  out[1] = (tag >> 8) & 255;
  out[2] = (tag >> 16) & 255;
  out[3] = 0x9d;
  out[4] = 0x01;
  out[5] = 0x2a;
  out[6] = header_w & 255;
  out[7] = (header_w >> 8) & 0x3f;
  out[8] = header_h & 255;
  out[9] = (header_h >> 8) & 0x3f;
  memcpy(out.data() + 10, p1.data(), p1.size());
  if (two_partitions) {
    out[10 + p1len] = len0 & 255;
    out[10 + p1len + 1] = (len0 >> 8) & 255;
    out[10 + p1len + 2] = (len0 >> 16) & 255;
  }
  return out;
}

size_t GetVmDataBytes() {
  FILE *f = fopen("/proc/self/status", "r");
  if (!f) return 0;
  char line[256];
  size_t kb = 0;
  while (fgets(line, sizeof(line), f)) {
    if (sscanf(line, "VmData: %zu kB", &kb) == 1) break;
  }
  fclose(f);
  return kb * 1024;
}

// Issue: 571147583
// Verifies that when restarting decode threads fails on a key frame with a new
// resolution, the cached stream info dimensions (ctx->si.w, ctx->si.h) are
// cleared so that a subsequent decode still triggers a resolution change and
// reallocates frame buffers to match the temporary multithread row buffers.
TEST(DecodeAPI, Vp8MultiThreadedRestartResolutionChange) {
  constexpr int kBig = 256;
  constexpr int kSmallW = 16;
  constexpr int kSmallH = 64;
  const std::vector<uint8_t> good =
      MakeVp8KeyFrame(kBig, kBig, kBig, kBig, true, false);
  const std::vector<uint8_t> bad =
      MakeVp8KeyFrame(kBig, kBig, kBig, kBig, true, true);
  const std::vector<uint8_t> odd =
      MakeVp8KeyFrame(kBig, kBig, kSmallW, kSmallH, true, true);
  const std::vector<uint8_t> good_small =
      MakeVp8KeyFrame(kSmallW, kSmallH, kSmallW, kSmallH, true, false);

  vpx_codec_ctx_t dec;
  vpx_codec_dec_cfg_t dec_cfg = { /*threads=*/2, /*w=*/0, /*h=*/0 };
  ASSERT_EQ(vpx_codec_dec_init(&dec, &vpx_codec_vp8_dx_algo, &dec_cfg, 0),
            VPX_CODEC_OK);

  // Frame 0: valid 256x256 keyframe initializes 256x256 buffers and 1 worker.
  ASSERT_EQ(
      vpx_codec_decode(&dec, good.data(),
                       static_cast<unsigned int>(good.size()), nullptr, 0),
      VPX_CODEC_OK);

  // Frame 1: truncated 256x256 keyframe fails in vp8mt_decode_mb_rows, which
  // shuts down worker threads and sets restart_threads = 1.
  EXPECT_EQ(vpx_codec_decode(&dec, bad.data(),
                             static_cast<unsigned int>(bad.size()), nullptr, 0),
            VPX_CODEC_CORRUPT_FRAME);

  auto decode_with_thread_start_failure = [&]() {
    // Hold cached pthread stacks with helper decoders that have already
    // started their worker threads, so pthread_create in
    // vp8_decoder_create_threads must mmap a new 8 MiB stack.
    constexpr int kNumHolders = 6;
    vpx_codec_ctx_t holders[kNumHolders];
    for (int i = 0; i < kNumHolders; ++i) {
      ASSERT_EQ(
          vpx_codec_dec_init(&holders[i], &vpx_codec_vp8_dx_algo, &dec_cfg, 0),
          VPX_CODEC_OK);
      ASSERT_EQ(vpx_codec_decode(&holders[i], good_small.data(),
                                 static_cast<unsigned int>(good_small.size()),
                                 nullptr, 0),
                VPX_CODEC_OK);
    }
    struct rlimit old_lim;
    ASSERT_EQ(getrlimit(RLIMIT_DATA, &old_lim), 0);
    const size_t vm_data = GetVmDataBytes();
    if (vm_data > 0) {
      struct rlimit tight_lim = old_lim;
      tight_lim.rlim_cur = vm_data + (4 << 20);
      ASSERT_EQ(setrlimit(RLIMIT_DATA, &tight_lim), 0);
    }
    const vpx_codec_err_t res = vpx_codec_decode(
        &dec, odd.data(), static_cast<unsigned int>(odd.size()), nullptr, 0);
    ASSERT_EQ(setrlimit(RLIMIT_DATA, &old_lim), 0);
    for (int i = 0; i < kNumHolders; ++i) {
      EXPECT_EQ(vpx_codec_destroy(&holders[i]), VPX_CODEC_OK);
    }
    EXPECT_EQ(res, VPX_CODEC_ERROR);
  };

  // Frame 2: 16x64 keyframe header while thread restart fails.
  decode_with_thread_start_failure();

  // Frame 3: 16x64 keyframe header; thread restart succeeds, decoding fails.
  EXPECT_EQ(vpx_codec_decode(&dec, odd.data(),
                             static_cast<unsigned int>(odd.size()), nullptr, 0),
            VPX_CODEC_CORRUPT_FRAME);

  // Frame 4: 16x64 keyframe header while thread restart fails again.
  decode_with_thread_start_failure();

  // Frame 5: 16x64 keyframe header; thread restart succeeds. Without clearing
  // ctx->si on thread restart failure, mt_yabove_row is allocated for width 16
  // while yv12_fb_new->y_width remains 256, overflowing mt_yabove_row.
  EXPECT_EQ(vpx_codec_decode(&dec, odd.data(),
                             static_cast<unsigned int>(odd.size()), nullptr, 0),
            VPX_CODEC_CORRUPT_FRAME);

  // Frame 6: valid 16x64 keyframe should decode and output a 16x64 frame.
  EXPECT_EQ(vpx_codec_decode(&dec, good_small.data(),
                             static_cast<unsigned int>(good_small.size()),
                             nullptr, 0),
            VPX_CODEC_OK);
  vpx_codec_iter_t iter = nullptr;
  const vpx_image_t *img = vpx_codec_get_frame(&dec, &iter);
  ASSERT_NE(img, nullptr);
  EXPECT_EQ(img->d_w, static_cast<unsigned int>(kSmallW));
  EXPECT_EQ(img->d_h, static_cast<unsigned int>(kSmallH));

  EXPECT_EQ(vpx_codec_destroy(&dec), VPX_CODEC_OK);
}
#endif  // CONFIG_MULTITHREAD && defined(__linux__)
#endif  // CONFIG_VP8_DECODER

#if CONFIG_VP9_DECODER
// Test VP9 codec controls after a decode error to ensure the code doesn't
// misbehave.
void TestVp9Controls(vpx_codec_ctx_t *dec) {
  static const int kControls[] = { VP8D_GET_LAST_REF_UPDATES,
                                   VP8D_GET_FRAME_CORRUPTED,
                                   VP9D_GET_DISPLAY_SIZE, VP9D_GET_FRAME_SIZE };
  int val[2];

  for (int i = 0; i < NELEMENTS(kControls); ++i) {
    const vpx_codec_err_t res = vpx_codec_control_(dec, kControls[i], val);
    switch (kControls[i]) {
      case VP8D_GET_FRAME_CORRUPTED:
        EXPECT_EQ(VPX_CODEC_ERROR, res) << kControls[i];
        break;
      default: EXPECT_EQ(VPX_CODEC_OK, res) << kControls[i]; break;
    }
    EXPECT_EQ(VPX_CODEC_INVALID_PARAM,
              vpx_codec_control_(dec, kControls[i], nullptr));
  }

  vp9_ref_frame_t ref;
  ref.idx = 0;
  EXPECT_EQ(VPX_CODEC_ERROR, vpx_codec_control(dec, VP9_GET_REFERENCE, &ref));
  EXPECT_EQ(VPX_CODEC_INVALID_PARAM,
            vpx_codec_control(dec, VP9_GET_REFERENCE, nullptr));

  vpx_ref_frame_t ref_copy;
  const int width = 352;
  const int height = 288;
  EXPECT_NE(vpx_img_alloc(&ref_copy.img, VPX_IMG_FMT_I420, width, height, 1),
            nullptr);
  ref_copy.frame_type = VP8_LAST_FRAME;
  EXPECT_EQ(VPX_CODEC_ERROR,
            vpx_codec_control(dec, VP8_COPY_REFERENCE, &ref_copy));
  EXPECT_EQ(VPX_CODEC_INVALID_PARAM,
            vpx_codec_control(dec, VP8_COPY_REFERENCE, nullptr));
  vpx_img_free(&ref_copy.img);
}

TEST(DecodeAPI, Vp9InvalidDecode) {
  vpx_codec_iface_t *const codec = &vpx_codec_vp9_dx_algo;
  const char filename[] =
      "invalid-vp90-2-00-quantizer-00.webm.ivf.s5861_r01-05_b6-.v2.ivf";
  libvpx_test::IVFVideoSource video(filename);
  video.Init();
  video.Begin();
  ASSERT_TRUE(!HasFailure());

  vpx_codec_ctx_t dec;
  EXPECT_EQ(VPX_CODEC_OK, vpx_codec_dec_init(&dec, codec, nullptr, 0));
  const uint32_t frame_size = static_cast<uint32_t>(video.frame_size());
#if CONFIG_VP9_HIGHBITDEPTH
  EXPECT_EQ(VPX_CODEC_MEM_ERROR,
            vpx_codec_decode(&dec, video.cxdata(), frame_size, nullptr, 0));
#else
  EXPECT_EQ(VPX_CODEC_UNSUP_BITSTREAM,
            vpx_codec_decode(&dec, video.cxdata(), frame_size, nullptr, 0));
#endif
  vpx_codec_iter_t iter = nullptr;
  EXPECT_EQ(nullptr, vpx_codec_get_frame(&dec, &iter));

  TestVp9Controls(&dec);
  EXPECT_EQ(VPX_CODEC_OK, vpx_codec_destroy(&dec));
}

void TestPeekInfo(const uint8_t *const data, uint32_t data_sz,
                  uint32_t peek_size) {
  vpx_codec_iface_t *const codec = &vpx_codec_vp9_dx_algo;
  // Verify behavior of vpx_codec_decode. vpx_codec_decode doesn't even get
  // to decoder_peek_si_internal on frames of size < 8.
  if (data_sz >= 8) {
    vpx_codec_ctx_t dec;
    EXPECT_EQ(VPX_CODEC_OK, vpx_codec_dec_init(&dec, codec, nullptr, 0));
    EXPECT_EQ((data_sz < peek_size) ? VPX_CODEC_UNSUP_BITSTREAM
                                    : VPX_CODEC_CORRUPT_FRAME,
              vpx_codec_decode(&dec, data, data_sz, nullptr, 0));
    vpx_codec_iter_t iter = nullptr;
    EXPECT_EQ(nullptr, vpx_codec_get_frame(&dec, &iter));
    EXPECT_EQ(VPX_CODEC_OK, vpx_codec_destroy(&dec));
  }

  // Verify behavior of vpx_codec_peek_stream_info.
  vpx_codec_stream_info_t si;
  si.sz = sizeof(si);
  EXPECT_EQ((data_sz < peek_size) ? VPX_CODEC_UNSUP_BITSTREAM : VPX_CODEC_OK,
            vpx_codec_peek_stream_info(codec, data, data_sz, &si));
}

TEST(DecodeAPI, Vp9PeekStreamInfo) {
  // The first 9 bytes are valid and the rest of the bytes are made up. Until
  // size 10, this should return VPX_CODEC_UNSUP_BITSTREAM and after that it
  // should return VPX_CODEC_CORRUPT_FRAME.
  const uint8_t data[32] = {
    0x85, 0xa4, 0xc1, 0xa1, 0x38, 0x81, 0xa3, 0x49, 0x83, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  };

  for (uint32_t data_sz = 1; data_sz <= 32; ++data_sz) {
    TestPeekInfo(data, data_sz, 10);
  }
}

TEST(DecodeAPI, Vp9PeekStreamInfoTruncated) {
  // This profile 1 header requires 10.25 bytes, ensure
  // vpx_codec_peek_stream_info doesn't over read.
  const uint8_t profile1_data[10] = { 0xa4, 0xe9, 0x30, 0x68, 0x53,
                                      0xe9, 0x30, 0x68, 0x53, 0x04 };

  for (uint32_t data_sz = 1; data_sz <= 10; ++data_sz) {
    TestPeekInfo(profile1_data, data_sz, 11);
  }
}

TEST(DecodeAPI, Buganizer499206650) {
  vpx_codec_ctx_t dec;
  ASSERT_EQ(vpx_codec_dec_init(&dec, vpx_codec_vp9_dx(), /*cfg=*/nullptr,
                               /*flags=*/0),
            VPX_CODEC_OK);

  // Frame 1: VP9 intra_only, profile 0, 8x8, error_resilient=1,
  // refresh_frame_flags=0x00. Clears need_resync without updating ref slots.
  static constexpr std::array<uint8_t, 83> frame1 = {
    0x85, 0xa4, 0xc1, 0xa1, 0x00, 0x00, 0x03, 0x80, 0x03, 0x80, 0x00, 0x40,
    0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  };
  EXPECT_EQ(vpx_codec_decode(&dec, frame1.data(),
                             static_cast<unsigned int>(frame1.size()),
                             /*user_priv=*/nullptr, /*deadline=*/0),
            VPX_CODEC_OK)
      << vpx_codec_error_detail(&dec);

  vpx_codec_iter_t it = nullptr;
  while (vpx_codec_get_frame(&dec, &it)) {
  }

  // Frame 2: VP9 inter, profile 0, show_frame=1, error_resilient=1,
  // refresh=0xFF, references slot 0 (ref_frame_map[0] == -1).
  static constexpr std::array<uint8_t, 16> frame2 = {
    0x87, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  };
  // Ensure there is no out of bounds read using the invalid index and that the
  // decoder reports an error.
  EXPECT_EQ(vpx_codec_decode(&dec, frame2.data(),
                             static_cast<unsigned int>(frame2.size()),
                             /*user_priv=*/nullptr, /*deadline=*/0),
            VPX_CODEC_CORRUPT_FRAME);

  EXPECT_EQ(vpx_codec_destroy(&dec), VPX_CODEC_OK);
}
#endif  // CONFIG_VP9_DECODER

TEST(DecodeAPI, HighBitDepthCapability) {
// VP8 should not claim VP9 HBD as a capability.
#if CONFIG_VP8_DECODER
  const vpx_codec_caps_t vp8_caps = vpx_codec_get_caps(&vpx_codec_vp8_dx_algo);
  EXPECT_EQ(vp8_caps & VPX_CODEC_CAP_HIGHBITDEPTH, 0);
#endif

#if CONFIG_VP9_DECODER
  const vpx_codec_caps_t vp9_caps = vpx_codec_get_caps(&vpx_codec_vp9_dx_algo);
#if CONFIG_VP9_HIGHBITDEPTH
  EXPECT_EQ(vp9_caps & VPX_CODEC_CAP_HIGHBITDEPTH, VPX_CODEC_CAP_HIGHBITDEPTH);
#else
  EXPECT_EQ(vp9_caps & VPX_CODEC_CAP_HIGHBITDEPTH, 0);
#endif
#endif
}

}  // namespace
