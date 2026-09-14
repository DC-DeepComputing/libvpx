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
#endif  // CONFIG_VP8_ENCODER && CONFIG_MULTITHREAD

#if CONFIG_MULTITHREAD
// Feeds a multi-threaded decoder a frame with two token partitions whose second
// token partition (used by a worker thread) is truncated, and verifies that the
// multi-threaded decoder returns VPX_CODEC_CORRUPT_FRAME and does not emit a
// partially written frame. Also verifies that a subsequent well-formed frame
// still decodes correctly.
TEST(DecodeAPI, Vp8MultiThreadedWorkerDecodeErrorFailsFrame) {
  // A 32x32 key frame with two token partitions where the second partition is
  // truncated to 1 byte, causing the worker thread decoding macroblock row 1
  // to fail.
  static const uint8_t kCorruptedFrame[250] = {
    0xb0, 0x07, 0x00, 0x9d, 0x01, 0x2a, 0x20, 0x00, 0x20, 0x00, 0x07, 0x87,
    0x08, 0x85, 0x85, 0x88, 0x99, 0x84, 0x88, 0x75, 0x02, 0xee, 0x42, 0x81,
    0x8b, 0x1c, 0x51, 0x90, 0x07, 0x58, 0x24, 0x76, 0x7c, 0x25, 0x90, 0xeb,
    0xb8, 0xd3, 0x0d, 0xe2, 0xff, 0x40, 0x3b, 0x60, 0x79, 0xe9, 0x7d, 0x00,
    0xef, 0x00, 0xf3, 0xf0, 0x7b, 0x0d, 0x7f, 0x81, 0x7d, 0x07, 0x9d, 0x0b,
    0x46, 0x23, 0x95, 0xa6, 0x0a, 0xf3, 0x31, 0x0e, 0x4a, 0xb4, 0x00, 0xaf,
    0x00, 0x00, 0xcc, 0x54, 0x93, 0x1e, 0xc3, 0xc1, 0x49, 0x6e, 0x5b, 0xb1,
    0x80, 0x01, 0x04, 0xf1, 0x6d, 0x6d, 0x35, 0xfd, 0x42, 0xd8, 0xdb, 0xce,
    0x1f, 0x70, 0x9b, 0xd4, 0x2f, 0x5d, 0xda, 0x4f, 0xb2, 0xe2, 0x59, 0x71,
    0x6d, 0x28, 0x90, 0x49, 0x72, 0x71, 0x41, 0x48, 0x53, 0x96, 0xb2, 0x02,
    0xb5, 0x40, 0x1c, 0x1c, 0xc4, 0x9a, 0x3b, 0x2c, 0xaf, 0x90, 0xc4, 0x34,
    0xbd, 0xa4, 0x4d, 0x2d, 0x8a, 0xf0, 0x37, 0xb0, 0x2b, 0x4b, 0xf8, 0x4d,
    0xc9, 0x24, 0xb4, 0xfa, 0x67, 0xa5, 0x07, 0xcf, 0x34, 0xc8, 0xf8, 0x79,
    0x91, 0x46, 0x4a, 0x3d, 0x32, 0xbb, 0x7f, 0xe1, 0xce, 0xb0, 0xa4, 0x10,
    0x34, 0x53, 0xe9, 0x54, 0x0e, 0xb4, 0x17, 0xad, 0x88, 0x73, 0x25, 0xb8,
    0xe8, 0x11, 0xd7, 0x53, 0x21, 0xc3, 0xa9, 0x17, 0x8b, 0xd3, 0xe9, 0x6a,
    0x4f, 0x76, 0x48, 0x71, 0x78, 0xde, 0x10, 0x52, 0xb6, 0x78, 0xc1, 0xbc,
    0x51, 0xd6, 0x6f, 0xf2, 0xcd, 0x51, 0x9d, 0x5f, 0x57, 0xd2, 0x9c, 0x92,
    0x4c, 0x02, 0xaa, 0x61, 0x64, 0x19, 0xce, 0x20, 0x90, 0xa5, 0xab, 0x1b,
    0xf0, 0x9e, 0xc7, 0x47, 0x7d, 0xc9, 0xe7, 0xe5, 0x45, 0x32, 0x90, 0x94,
    0xea, 0x48, 0xda, 0xc7, 0x16, 0xc9, 0x59, 0x14, 0x20, 0xfc,
  };

  // A 32x32 well-formed key frame with two token partitions.
  static const uint8_t kWellFormedFrame[415] = {
    0x50, 0x07, 0x00, 0x9d, 0x01, 0x2a, 0x20, 0x00, 0x20, 0x00, 0x0d, 0x47,
    0x08, 0x85, 0x85, 0x88, 0x99, 0x84, 0x88, 0x75, 0x02, 0xee, 0x42, 0x81,
    0x8b, 0x1c, 0x51, 0x90, 0x07, 0x58, 0x24, 0x7a, 0xfc, 0x1a, 0xb9, 0x0a,
    0x35, 0xc2, 0x46, 0x8a, 0x8b, 0xf0, 0x15, 0x83, 0x78, 0x43, 0xd0, 0x0f,
    0xfc, 0xdd, 0xd0, 0x3c, 0xf7, 0x5e, 0x80, 0x7c, 0xf0, 0x3a, 0x80, 0x77,
    0x85, 0x7f, 0xc2, 0x8d, 0x1e, 0x16, 0x0e, 0x90, 0xb2, 0x00, 0x00, 0xf8,
    0xfd, 0x6c, 0xcb, 0x50, 0x83, 0xd4, 0x44, 0x32, 0xcf, 0x35, 0xed, 0x33,
    0x11, 0xe1, 0xf0, 0xa6, 0x59, 0x32, 0xd3, 0xcc, 0xfd, 0x05, 0x61, 0x58,
    0x45, 0xeb, 0x0b, 0x76, 0xbf, 0xb6, 0xe0, 0xfd, 0x60, 0xb3, 0xb0, 0x35,
    0x56, 0x88, 0x96, 0x90, 0x46, 0xa2, 0xb9, 0xd5, 0x44, 0x93, 0x90, 0x5b,
    0xd2, 0x46, 0xa8, 0x0d, 0x41, 0x2a, 0xbf, 0x5b, 0x86, 0xea, 0x2b, 0xf9,
    0xfc, 0x65, 0x5f, 0x89, 0xf8, 0x0b, 0xdc, 0xb1, 0xff, 0xfa, 0x07, 0xbe,
    0xbc, 0x99, 0x3c, 0xe0, 0x7a, 0xb3, 0x28, 0xa4, 0xb0, 0x25, 0xdd, 0xb9,
    0xad, 0xd8, 0xd7, 0x2b, 0xb1, 0x9b, 0x47, 0x9c, 0xb1, 0x5e, 0x10, 0x4b,
    0x43, 0x68, 0xfc, 0x9a, 0xf7, 0xe0, 0xb0, 0x14, 0x9e, 0x7b, 0xed, 0xe9,
    0x4d, 0xc5, 0x49, 0x4e, 0x78, 0x0f, 0xbf, 0xf3, 0xc1, 0x1e, 0x0d, 0xc6,
    0xf6, 0xe1, 0x8e, 0x3e, 0x36, 0xf5, 0xa1, 0xb5, 0xf0, 0x86, 0xb7, 0x7e,
    0x71, 0xf6, 0x5c, 0xa9, 0x6e, 0x7a, 0x74, 0xa3, 0xc1, 0x52, 0x7f, 0x07,
    0x92, 0x5c, 0xbf, 0xe8, 0x0e, 0x08, 0x7f, 0xed, 0xe9, 0x9c, 0x02, 0x93,
    0x72, 0xae, 0x60, 0x50, 0xd8, 0xdf, 0xce, 0x92, 0x37, 0x10, 0x50, 0xcf,
    0xa9, 0x96, 0x52, 0x0c, 0x81, 0x4b, 0x35, 0xc2, 0xa0, 0x8a, 0xc8, 0xc7,
    0x22, 0xac, 0xea, 0x57, 0x2c, 0xf4, 0x68, 0x12, 0x54, 0x60, 0x7f, 0xba,
    0xba, 0xa2, 0xf3, 0x37, 0x61, 0x3a, 0x78, 0x5e, 0xbc, 0xcc, 0x55, 0x04,
    0x5a, 0x9b, 0x04, 0xb2, 0x8d, 0x96, 0xe2, 0x2e, 0xa7, 0x34, 0x29, 0x97,
    0x89, 0x55, 0x23, 0xc4, 0xae, 0x47, 0x41, 0xf8, 0x9b, 0x56, 0xb9, 0xd9,
    0x1d, 0x2b, 0x19, 0xeb, 0x40, 0x94, 0x17, 0xf2, 0x71, 0x15, 0xb8, 0xea,
    0xa9, 0x91, 0xb4, 0x6d, 0xd0, 0xa9, 0xef, 0xc4, 0xb7, 0x08, 0xa0, 0x6b,
    0xb0, 0xa8, 0xc9, 0x3f, 0x1d, 0x00, 0x68, 0xc1, 0x93, 0x56, 0x9d, 0x2b,
    0xa4, 0x0f, 0xe5, 0xde, 0x09, 0x1c, 0xe0, 0xbe, 0xd6, 0x27, 0x29, 0x12,
    0x28, 0x5d, 0xf2, 0xd0, 0xda, 0x50, 0x56, 0xab, 0xec, 0xa2, 0xe4, 0x10,
    0x2b, 0xd8, 0xa0, 0x23, 0x9b, 0xec, 0x28, 0xd4, 0xbe, 0x9e, 0x92, 0x87,
    0x4e, 0x62, 0x57, 0xe5, 0x6a, 0x92, 0x09, 0x49, 0x59, 0xb3, 0x0e, 0xd1,
    0x84, 0x23, 0x32, 0x76, 0x88, 0xb8, 0x23, 0xdf, 0x79, 0x1b, 0x8c, 0xf8,
    0xb7, 0xbd, 0x4c, 0xd6, 0x07, 0x84, 0xad, 0x64, 0x59, 0xbf, 0x5b, 0x0d,
    0x66, 0xea, 0x8c, 0x3e, 0xfe, 0xde, 0xc0,
  };

  const struct {
    const uint8_t *data;
    size_t size;
    vpx_codec_err_t expected_res;
  } kTestFrames[] = {
    { kWellFormedFrame, sizeof(kWellFormedFrame), VPX_CODEC_OK },
    { kCorruptedFrame, sizeof(kCorruptedFrame), VPX_CODEC_CORRUPT_FRAME },
    { kWellFormedFrame, sizeof(kWellFormedFrame), VPX_CODEC_OK },
  };

  vpx_codec_ctx_t dec;
  vpx_codec_dec_cfg_t dec_cfg = { /*threads=*/4, /*w=*/0, /*h=*/0 };
  ASSERT_EQ(vpx_codec_dec_init(&dec, &vpx_codec_vp8_dx_algo, &dec_cfg, 0),
            VPX_CODEC_OK);

  for (size_t i = 0; i < sizeof(kTestFrames) / sizeof(kTestFrames[0]); ++i) {
    const vpx_codec_err_t res =
        vpx_codec_decode(&dec, kTestFrames[i].data,
                         static_cast<unsigned int>(kTestFrames[i].size),
                         /*user_priv=*/nullptr, /*deadline=*/0);
    vpx_codec_iter_t iter = nullptr;
    const vpx_image_t *img = vpx_codec_get_frame(&dec, &iter);
    EXPECT_EQ(res, kTestFrames[i].expected_res)
        << "frame " << i << ": " << vpx_codec_error_detail(&dec);
    if (kTestFrames[i].expected_res == VPX_CODEC_CORRUPT_FRAME) {
      EXPECT_EQ(img, nullptr) << "frame " << i;
    } else {
      EXPECT_NE(img, nullptr) << "frame " << i;
      int corrupted = -1;
      EXPECT_EQ(vpx_codec_control(&dec, VP8D_GET_FRAME_CORRUPTED, &corrupted),
                VPX_CODEC_OK);
      EXPECT_EQ(corrupted, 0) << "frame " << i;
    }
  }
  EXPECT_EQ(vpx_codec_destroy(&dec), VPX_CODEC_OK);
}
#endif  // CONFIG_MULTITHREAD
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
