/*
 *  Copyright (c) 2026 The WebM project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#include <stddef.h>

#include "gtest/gtest.h"

#include "vpx/vpx_integer.h"
#include "vpx_dsp/bytereader.h"

namespace {

const uint8_t kData[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08 };

TEST(ByteReaderTest, ReadsLittleAndBigEndian) {
  vpx_byte_reader r;
  vpx_byte_reader_init(&r, kData, sizeof(kData), nullptr, nullptr);
  EXPECT_EQ(vpx_byte_reader_bytes_left(&r), 8u);
  EXPECT_EQ(vpx_byte_reader_read_u8(&r), 0x01);
  EXPECT_EQ(vpx_byte_reader_read_le(&r, 2), 0x0302u);
  EXPECT_EQ(vpx_byte_reader_read_be(&r, 4), 0x04050607u);
  EXPECT_EQ(vpx_byte_reader_read_le(&r, 1), 0x08u);
  EXPECT_EQ(vpx_byte_reader_bytes_left(&r), 0u);
  EXPECT_EQ(r.error, 0);
}

TEST(ByteReaderTest, AllWidths) {
  for (int n = 1; n <= 4; ++n) {
    vpx_byte_reader r;
    vpx_byte_reader_init(&r, kData, sizeof(kData), nullptr, nullptr);
    uint32_t le = 0, be = 0;
    for (int i = 0; i < n; ++i) {
      le |= static_cast<uint32_t>(kData[i]) << (8 * i);
      be = (be << 8) | kData[n + i];
    }
    EXPECT_EQ(vpx_byte_reader_read_le(&r, n), le) << n;
    EXPECT_EQ(vpx_byte_reader_read_be(&r, n), be) << n;
    EXPECT_EQ(vpx_byte_reader_bytes_left(&r), sizeof(kData) - 2 * n);
    EXPECT_EQ(r.error, 0);
  }
}

TEST(ByteReaderTest, ShortReadFailsAndIsSticky) {
  vpx_byte_reader r;
  vpx_byte_reader_init(&r, kData, 3, nullptr, nullptr);
  EXPECT_EQ(vpx_byte_reader_read_le(&r, 4), 0u);
  EXPECT_EQ(r.error, 1);
  EXPECT_EQ(vpx_byte_reader_bytes_left(&r), 0u);
  EXPECT_EQ(vpx_byte_reader_ptr(&r), kData + 3);
  // Reads after a failure keep failing, even ones that would have fit.
  EXPECT_EQ(vpx_byte_reader_read_u8(&r), 0);
  EXPECT_EQ(r.error, 1);
}

TEST(ByteReaderTest, ReadBytesZeroesOutputOnFailure) {
  vpx_byte_reader r;
  uint8_t out[4] = { 0xff, 0xff, 0xff, 0xff };
  vpx_byte_reader_init(&r, kData, 2, nullptr, nullptr);
  EXPECT_EQ(vpx_byte_reader_read_bytes(&r, out, sizeof(out)), 0);
  for (uint8_t b : out) EXPECT_EQ(b, 0);
  EXPECT_EQ(r.error, 1);
}

TEST(ByteReaderTest, TakeAndSkip) {
  vpx_byte_reader r;
  vpx_byte_reader_init(&r, kData, sizeof(kData), nullptr, nullptr);
  EXPECT_EQ(vpx_byte_reader_take(&r, 3), kData);
  EXPECT_EQ(vpx_byte_reader_take(&r, 0), kData + 3);
  EXPECT_EQ(vpx_byte_reader_skip(&r, 4), 1);
  EXPECT_EQ(vpx_byte_reader_ptr(&r), kData + 7);
  EXPECT_EQ(vpx_byte_reader_take(&r, 1), kData + 7);
  EXPECT_EQ(vpx_byte_reader_take(&r, 0), kData + 8);
  EXPECT_EQ(r.error, 0);
  EXPECT_EQ(vpx_byte_reader_take(&r, 1), nullptr);
  EXPECT_EQ(r.error, 1);
}

TEST(ByteReaderTest, LargeCountsFail) {
  vpx_byte_reader r;
  vpx_byte_reader_init(&r, kData, sizeof(kData), nullptr, nullptr);
  EXPECT_EQ(vpx_byte_reader_skip(&r, 1), 1);
  EXPECT_EQ(vpx_byte_reader_skip(&r, SIZE_MAX), 0);
  EXPECT_EQ(r.error, 1);
  EXPECT_EQ(vpx_byte_reader_ptr(&r), kData + sizeof(kData));

  vpx_byte_reader_init(&r, kData, sizeof(kData), nullptr, nullptr);
  EXPECT_EQ(vpx_byte_reader_take(&r, sizeof(kData) + 1), nullptr);
  EXPECT_EQ(r.error, 1);
}

TEST(ByteReaderTest, EmptyReader) {
  vpx_byte_reader r;
  vpx_byte_reader_init(&r, nullptr, 0, nullptr, nullptr);
  EXPECT_EQ(vpx_byte_reader_bytes_left(&r), 0u);
  EXPECT_EQ(vpx_byte_reader_skip(&r, 0), 1);
  EXPECT_EQ(r.error, 0);
  EXPECT_EQ(vpx_byte_reader_read_u8(&r), 0);
  EXPECT_EQ(r.error, 1);
}

struct DecryptState {
  const uint8_t *base;
  int calls;
};

// Inverts every byte, and checks that the input lies within kData.
void InvertDecrypt(void *decrypt_state, const unsigned char *input,
                   unsigned char *output, int count) {
  DecryptState *const state = static_cast<DecryptState *>(decrypt_state);
  ASSERT_GE(input, state->base);
  ASSERT_LE(input + count, state->base + sizeof(kData));
  for (int i = 0; i < count; ++i) output[i] = input[i] ^ 0xff;
  ++state->calls;
}

TEST(ByteReaderTest, DecryptsValuesButNotTakenBytes) {
  DecryptState state = { kData, 0 };
  vpx_byte_reader r;
  vpx_byte_reader_init(&r, kData, sizeof(kData), InvertDecrypt, &state);
  EXPECT_EQ(vpx_byte_reader_read_u8(&r), 0xfe);
  EXPECT_EQ(vpx_byte_reader_read_le(&r, 2), 0xfcfdu);
  EXPECT_EQ(state.calls, 2);
  // take() returns the raw bytes.
  EXPECT_EQ(vpx_byte_reader_take(&r, 1), kData + 3);
  EXPECT_EQ(vpx_byte_reader_read_be(&r, 3), 0xfaf9f8u);
  EXPECT_EQ(state.calls, 3);
  // A failed read does not call the decrypt callback.
  EXPECT_EQ(vpx_byte_reader_read_le(&r, 2), 0u);
  EXPECT_EQ(state.calls, 3);
  EXPECT_EQ(r.error, 1);
}

}  // namespace
