/*
 *  Copyright (c) 2026 The WebM project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#include <riscv_vector.h>

#include "./vpx_config.h"
#include "./vpx_dsp_rtcd.h"
#include "vpx/vpx_integer.h"
#include "vpx_dsp/vpx_filter.h"

/* Row cores shared with the single-pass RTCD kernels. */
void rvv_convolve8_horiz_rows(const uint8_t *src, ptrdiff_t src_stride,
                              uint8_t *dst, ptrdiff_t dst_stride,
                              const int16_t *f, int w, int h);
void rvv_convolve8_vert_rows(const uint8_t *src, ptrdiff_t src_stride,
                             uint8_t *dst, ptrdiff_t dst_stride,
                             const int16_t *f, int w, int h);

/* Two-pass subpel convolution (unit step fast path), bit-exact vs
 * vpx_convolve8_c: horizontal into a fixed-stride intermediate buffer,
 * then vertical to dst. Both passes run the RVV row cores shared with
 * the single-pass RTCD slots. Any scaling falls back to the C path. */
void vpx_convolve8_rvv(const uint8_t *src, ptrdiff_t src_stride, uint8_t *dst,
                       ptrdiff_t dst_stride, const InterpKernel *filter,
                       int x0_q4, int x_step_q4, int y0_q4, int y_step_q4,
                       int w, int h) {
  if (x_step_q4 != 16 || y_step_q4 != 16) {
    vpx_convolve8_c(src, src_stride, dst, dst_stride, filter, x0_q4, x_step_q4,
                    y0_q4, y_step_q4, w, h);
    return;
  }

  /* Intermediate buffer, identical layout to vpx_convolve8_c: stride 64,
   * up to ((64-1)*16 + 15)>>4 + 8 = 79 rows for unit step (h <= 64). */
  uint8_t temp[64 * 135];
  const int intermediate_height =
      (((h - 1) * y_step_q4 + y0_q4) >> SUBPEL_BITS) + SUBPEL_TAPS;

  /* Pass 1: horizontal into temp. C enters at src - 3 rows and its inner
   * core subtracts 3 more columns; the row core here takes the final base,
   * so pass row offset - 3 + (x0_q4>>4) and column offset - 3 combined. */
  rvv_convolve8_horiz_rows(src - src_stride * (SUBPEL_TAPS / 2 - 1) -
                               (SUBPEL_TAPS / 2 - 1) +
                               (x0_q4 >> SUBPEL_BITS),
                           src_stride, temp, 64, filter[x0_q4 & SUBPEL_MASK], w,
                           intermediate_height);

  /* Pass 2: vertical from temp. C enters at temp + 64*3 and its inner core
   * subtracts 3 rows (net: temp + 0); the row core here takes the final
   * base, so only the integer-step row offset (y0_q4>>4) remains. */
  rvv_convolve8_vert_rows(temp + 64 * (y0_q4 >> SUBPEL_BITS), 64, dst,
                          dst_stride, filter[y0_q4 & SUBPEL_MASK], w, h);
}
