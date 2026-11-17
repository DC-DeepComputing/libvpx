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

static void copy_row_rvv(const uint8_t *src, uint8_t *dst, int w) {
  const uint8_t *s = src;
  uint8_t *d = dst;
  size_t len = (size_t)w;

  while (len > 0) {
    size_t vl = __riscv_vsetvl_e8m4(len);
    vuint8m4_t v = __riscv_vle8_v_u8m4(s, vl);
    __riscv_vse8_v_u8m4(d, v, vl);
    s += vl;
    d += vl;
    len -= vl;
  }
}

void vpx_convolve_copy_rvv(const uint8_t *src, ptrdiff_t src_stride,
                           uint8_t *dst, ptrdiff_t dst_stride,
                           const InterpKernel *filter, int x0_q4, int x_step_q4,
                           int y0_q4, int y_step_q4, int w, int h) {
  (void)filter;
  (void)x0_q4;
  (void)x_step_q4;
  (void)y0_q4;
  (void)y_step_q4;

  while (h-- > 0) {
    copy_row_rvv(src, dst, w);
    src += src_stride;
    dst += dst_stride;
  }
}
