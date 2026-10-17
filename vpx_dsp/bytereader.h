/*
 *  Copyright (c) 2026 The WebM project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#ifndef VPX_VPX_DSP_BYTEREADER_H_
#define VPX_VPX_DSP_BYTEREADER_H_

#include <assert.h>
#include <limits.h>
#include <stddef.h>
#include <string.h>

#include "./vpx_config.h"
#include "vpx/vp8dx.h"
#include "vpx/vpx_integer.h"

#ifdef __cplusplus
extern "C" {
#endif

// A bounded, forward-only reader for byte-aligned fields in compressed data
// (frame tags, partition and tile sizes, superframe indices).
//
// Every read checks the number of bytes remaining. A read or skip that would
// pass the end of the buffer sets |error| and moves the reader to the end of
// the buffer, so all later reads also fail; the read functions return 0 in
// that case. Callers can test vpx_byte_reader_bytes_left() before reading or
// test |error| once after a group of reads.
//
// If |decrypt_cb| is set, the value-returning functions decrypt the bytes they
// read. vpx_byte_reader_take() returns a pointer to the raw (still encrypted)
// bytes, for code that decrypts them itself, such as the bool decoders.
struct vpx_byte_reader {
  const uint8_t *buf;  // Next byte to read.
  size_t size;         // Number of readable bytes at |buf|.
  vpx_decrypt_cb decrypt_cb;
  void *decrypt_state;
  int error;
};

static INLINE void vpx_byte_reader_init(struct vpx_byte_reader *r,
                                        const uint8_t *data, size_t size,
                                        vpx_decrypt_cb decrypt_cb,
                                        void *decrypt_state) {
  assert(data != NULL || size == 0);
  r->buf = data;
  r->size = size;
  r->decrypt_cb = decrypt_cb;
  r->decrypt_state = decrypt_state;
  r->error = 0;
}

static INLINE size_t
vpx_byte_reader_bytes_left(const struct vpx_byte_reader *r) {
  return r->size;
}

// Returns a pointer to the next unread byte.
static INLINE const uint8_t *vpx_byte_reader_ptr(
    const struct vpx_byte_reader *r) {
  return r->buf;
}

static INLINE void vpx_byte_reader_fail(struct vpx_byte_reader *r) {
  if (r->size > 0) r->buf += r->size;
  r->size = 0;
  r->error = 1;
}

// Advances past the next |n| bytes. Returns 0 if fewer than |n| bytes remain.
static INLINE int vpx_byte_reader_skip(struct vpx_byte_reader *r, size_t n) {
  if (n > r->size) {
    vpx_byte_reader_fail(r);
    return 0;
  }
  if (n > 0) {
    r->buf += n;
    r->size -= n;
  }
  return 1;
}

// Returns a pointer to the next |n| bytes and advances past them, or returns
// NULL if fewer than |n| bytes remain.
static INLINE const uint8_t *vpx_byte_reader_take(struct vpx_byte_reader *r,
                                                  size_t n) {
  const uint8_t *const p = r->buf;
  return vpx_byte_reader_skip(r, n) ? p : NULL;
}

// Copies (and, if needed, decrypts) the next |n| bytes to |out|. Returns 0 and
// leaves |out| zeroed if fewer than |n| bytes remain.
static INLINE int vpx_byte_reader_read_bytes(struct vpx_byte_reader *r,
                                             uint8_t *out, size_t n) {
  if (n > r->size) {
    vpx_byte_reader_fail(r);
    memset(out, 0, n);
    return 0;
  }
  if (n > 0) {
    if (r->decrypt_cb) {
      assert(n <= INT_MAX);
      r->decrypt_cb(r->decrypt_state, r->buf, out, (int)n);
    } else {
      memcpy(out, r->buf, n);
    }
    r->buf += n;
    r->size -= n;
  }
  return 1;
}

static INLINE uint8_t vpx_byte_reader_read_u8(struct vpx_byte_reader *r) {
  uint8_t v;
  vpx_byte_reader_read_bytes(r, &v, 1);
  return v;
}

// Reads an |n| byte little-endian value, 1 <= n <= 4.
static INLINE uint32_t vpx_byte_reader_read_le(struct vpx_byte_reader *r,
                                               int n) {
  uint8_t b[4];
  uint32_t v = 0;
  int i;
  assert(n >= 1 && n <= 4);
  vpx_byte_reader_read_bytes(r, b, (size_t)n);
  for (i = n - 1; i >= 0; --i) v = (v << 8) | b[i];
  return v;
}

// Reads an |n| byte big-endian value, 1 <= n <= 4.
static INLINE uint32_t vpx_byte_reader_read_be(struct vpx_byte_reader *r,
                                               int n) {
  uint8_t b[4];
  uint32_t v = 0;
  int i;
  assert(n >= 1 && n <= 4);
  vpx_byte_reader_read_bytes(r, b, (size_t)n);
  for (i = 0; i < n; ++i) v = (v << 8) | b[i];
  return v;
}

#ifdef __cplusplus
}  // extern "C"
#endif

#endif  // VPX_VPX_DSP_BYTEREADER_H_
