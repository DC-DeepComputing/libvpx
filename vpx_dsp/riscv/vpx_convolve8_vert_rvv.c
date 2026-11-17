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

/* Core: vertical 8-tap rows with a preselected kernel, unit step.
 * Shared by vpx_convolve8_vert_rvv (RTCD slot) and vpx_convolve8_rvv
 * (two-pass). Bit-exact vs convolve_vert in vpx_convolve.c. */
void rvv_convolve8_vert_rows(const uint8_t *src, ptrdiff_t src_stride,
                             uint8_t *dst, ptrdiff_t dst_stride,
                             const int16_t *f, int w, int h) {
  for (int y = 0; y < h; ++y) {
    int x = 0;
    int rem = w;
    while (rem > 0) {
      const size_t vl = __riscv_vsetvl_e8m1(rem);
      vint32m4_t acc = __riscv_vmv_v_x_i32m4(0, vl);

      for (int k = 0; k < SUBPEL_TAPS; ++k) {
        const vuint8m1_t u =
            __riscv_vle8_v_u8m1(src + k * src_stride + x, vl);
        const vint16m2_t s = __riscv_vreinterpret_v_u16m2_i16m2(
            __riscv_vzext_vf2_u16m2(u, vl));
        if (k == 0)
          acc = __riscv_vwmul_vx_i32m4(s, f[k], vl);
        else
          acc = __riscv_vwmacc_vx_i32m4(acc, f[k], s, vl);
      }

      acc = __riscv_vadd_vx_i32m4(acc, 1 << (FILTER_BITS - 1), vl);
      acc = __riscv_vsra_vx_i32m4(acc, FILTER_BITS, vl);
      acc = __riscv_vmax_vx_i32m4(acc, 0, vl);
      acc = __riscv_vmin_vx_i32m4(acc, 255, vl);

      const vint16m2_t mid = __riscv_vncvt_x_x_w_i16m2(acc, vl);
      const vuint8m1_t out = __riscv_vncvt_x_x_w_u8m1(
          __riscv_vreinterpret_v_i16m2_u16m2(mid), vl);
      __riscv_vse8_v_u8m1(dst + x, out, vl);

      x += vl;
      rem -= vl;
    }
    src += src_stride;
    dst += dst_stride;
  }
}

void vpx_convolve8_vert_rvv(const uint8_t *src, ptrdiff_t src_stride,
                            uint8_t *dst, ptrdiff_t dst_stride,
                            const InterpKernel *filter, int x0_q4,
                            int x_step_q4, int y0_q4, int y_step_q4, int w,
                            int h) {
  if (y_step_q4 != 16) {
    vpx_convolve8_vert_c(src, src_stride, dst, dst_stride, filter, x0_q4,
                         x_step_q4, y0_q4, y_step_q4, w, h);
    return;
  }

  rvv_convolve8_vert_rows(
      src - src_stride * (SUBPEL_TAPS / 2 - 1) +
          src_stride * (y0_q4 >> SUBPEL_BITS),
      src_stride, dst, dst_stride, filter[y0_q4 & SUBPEL_MASK], w, h);
}
