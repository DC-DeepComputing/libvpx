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
#include "vpx_ports/mem.h"

/* RVV loop filter (horizontal edges), 8 columns per invocation (VLEN=256
 * e8m1 lanes 0..7). Bit-exact vs vpx_dsp/loopfilter.c:
 *
 *  - mask/flat/hev are per-lane boolean chains (absdiff via
 *    vssubu+vmaxu, comparisons via vmsgtu, combination via vmor/vmnot).
 *  - filter4 runs in the i16 domain: ps1-qs1 and 3*(qs0-ps0) exceed i8
 *    range, the C code clamps after int promotion; we widen with vsext,
 *    clamp with vmin/vmax, and narrow back with vncvt.
 *  - filter8's 7-tap averages run in u16 (sum <= 2040+4), then >>3.
 *  - per-lane branch (flat&&mask ? filter8 : filter4) is a vmerge;
 *    mask==0 reproduces the original bytes through filter4's algebra.
 */

#define LPF_VL 8

static INLINE vuint8m1_t lpf_absdiff_u8(vuint8m1_t a, vuint8m1_t b, size_t vl) {
  return __riscv_vmaxu_vv_u8m1(__riscv_vssubu_vv_u8m1(a, b, vl),
                               __riscv_vssubu_vv_u8m1(b, a, vl), vl);
}

static INLINE vint16m2_t lpf_sclamp16(vint16m2_t x, size_t vl) {
  return __riscv_vmin_vx_i16m2(
      __riscv_vmax_vx_i16m2(x, -128, vl), 127, vl);
}

static INLINE vint8m1_t lpf_narrow8(vint16m2_t x, size_t vl) {
  return __riscv_vncvt_x_x_w_i8m1(x, vl);
}

/* filter4 on vectors; writes op1/op0/q0/q1 as uint8 results. */
static INLINE void lpf_filter4(vbool8_t mask_b, vbool8_t hev_b, vuint8m1_t p1,
                               vuint8m1_t p0, vuint8m1_t q0, vuint8m1_t q1,
                               size_t vl, vuint8m1_t *out_op1,
                               vuint8m1_t *out_op0, vuint8m1_t *out_oq0,
                               vuint8m1_t *out_oq1) {
  const vint8m1_t ps1 =
      __riscv_vreinterpret_v_u8m1_i8m1(__riscv_vxor_vx_u8m1(p1, 0x80, vl));
  const vint8m1_t ps0 =
      __riscv_vreinterpret_v_u8m1_i8m1(__riscv_vxor_vx_u8m1(p0, 0x80, vl));
  const vint8m1_t qs0 =
      __riscv_vreinterpret_v_u8m1_i8m1(__riscv_vxor_vx_u8m1(q0, 0x80, vl));
  const vint8m1_t qs1 =
      __riscv_vreinterpret_v_u8m1_i8m1(__riscv_vxor_vx_u8m1(q1, 0x80, vl));

  const vint8m1_t zero8 = __riscv_vmv_v_x_i8m1(0, vl);

  /* filter = clamp(ps1 - qs1) & hev  (i16 domain, then narrow) */
  vint16m2_t f = lpf_sclamp16(
      __riscv_vsub_vv_i16m2(__riscv_vsext_vf2_i16m2(ps1, vl),
                            __riscv_vsext_vf2_i16m2(qs1, vl), vl),
      vl);
  vint8m1_t f8 = lpf_narrow8(f, vl);
  f8 = __riscv_vmerge_vvm_i8m1(zero8, f8, hev_b, vl);

  /* filter = clamp(filter + 3*(qs0 - ps0)) & mask */
  f = lpf_sclamp16(
      __riscv_vadd_vv_i16m2(
          __riscv_vsext_vf2_i16m2(f8, vl),
          __riscv_vmul_vx_i16m2(
              __riscv_vsub_vv_i16m2(__riscv_vsext_vf2_i16m2(qs0, vl),
                                    __riscv_vsext_vf2_i16m2(ps0, vl), vl),
              3, vl),
          vl),
      vl);
  f8 = lpf_narrow8(f, vl);
  f8 = __riscv_vmerge_vvm_i8m1(zero8, f8, mask_b, vl);

  /* filter1 = clamp(filter + 4) >> 3 ; filter2 = clamp(filter + 3) >> 3 */
  const vint16m2_t f1 = __riscv_vsra_vx_i16m2(
      lpf_sclamp16(__riscv_vadd_vx_i16m2(
                       __riscv_vsext_vf2_i16m2(f8, vl), 4, vl),
                   vl),
      3, vl);
  const vint16m2_t f2 = __riscv_vsra_vx_i16m2(
      lpf_sclamp16(__riscv_vadd_vx_i16m2(
                       __riscv_vsext_vf2_i16m2(f8, vl), 3, vl),
                   vl),
      3, vl);

  {
    const vint16m2_t q0m = lpf_sclamp16(
        __riscv_vsub_vv_i16m2(__riscv_vsext_vf2_i16m2(qs0, vl), f1, vl), vl);
    const vint16m2_t p0m = lpf_sclamp16(
        __riscv_vadd_vv_i16m2(__riscv_vsext_vf2_i16m2(ps0, vl), f2, vl), vl);
    const vuint8m1_t oq0r = __riscv_vreinterpret_v_i8m1_u8m1(
        lpf_narrow8(q0m, vl));
    const vuint8m1_t op0r = __riscv_vreinterpret_v_i8m1_u8m1(
        lpf_narrow8(p0m, vl));
    *out_oq0 = __riscv_vxor_vx_u8m1(oq0r, 0x80, vl);
    *out_op0 = __riscv_vxor_vx_u8m1(op0r, 0x80, vl);
  }

  /* filter = ROUND_POWER_OF_TWO(filter1, 1) & ~hev */
  vint16m2_t fo = __riscv_vsra_vx_i16m2(
      __riscv_vadd_vx_i16m2(f1, 1, vl), 1, vl);
  fo = __riscv_vmerge_vvm_i16m2(__riscv_vmv_v_x_i16m2(0, vl), fo,
                                __riscv_vmnot_m_b8(hev_b, vl), vl);

  {
    const vint16m2_t q1m = lpf_sclamp16(
        __riscv_vsub_vv_i16m2(__riscv_vsext_vf2_i16m2(qs1, vl), fo, vl), vl);
    const vint16m2_t p1m = lpf_sclamp16(
        __riscv_vadd_vv_i16m2(__riscv_vsext_vf2_i16m2(ps1, vl), fo, vl), vl);
    const vuint8m1_t oq1r = __riscv_vreinterpret_v_i8m1_u8m1(
        lpf_narrow8(q1m, vl));
    const vuint8m1_t op1r = __riscv_vreinterpret_v_i8m1_u8m1(
        lpf_narrow8(p1m, vl));
    *out_oq1 = __riscv_vxor_vx_u8m1(oq1r, 0x80, vl);
    *out_op1 = __riscv_vxor_vx_u8m1(op1r, 0x80, vl);
  }
}

/* filter8 7-tap outputs for op2..oq2 (u16 accumulate, >>3 with +4). */
static INLINE void lpf_filter8_taps(vuint8m1_t p3, vuint8m1_t p2,
                                    vuint8m1_t p1, vuint8m1_t p0,
                                    vuint8m1_t q0, vuint8m1_t q1,
                                    vuint8m1_t q2, vuint8m1_t q3, size_t vl,
                                    vuint8m1_t *op2, vuint8m1_t *op1,
                                    vuint8m1_t *op0, vuint8m1_t *oq0,
                                    vuint8m1_t *oq1, vuint8m1_t *oq2) {
  const vuint16m2_t w3 = __riscv_vzext_vf2_u16m2(p3, vl);
  const vuint16m2_t w2 = __riscv_vzext_vf2_u16m2(p2, vl);
  const vuint16m2_t w1 = __riscv_vzext_vf2_u16m2(p1, vl);
  const vuint16m2_t w0 = __riscv_vzext_vf2_u16m2(p0, vl);
  const vuint16m2_t v0 = __riscv_vzext_vf2_u16m2(q0, vl);
  const vuint16m2_t v1 = __riscv_vzext_vf2_u16m2(q1, vl);
  const vuint16m2_t v2 = __riscv_vzext_vf2_u16m2(q2, vl);
  const vuint16m2_t v3 = __riscv_vzext_vf2_u16m2(q3, vl);

  vuint16m2_t t;
  t = __riscv_vmul_vx_u16m2(w3, 3, vl);
  t = __riscv_vmacc_vx_u16m2(t, 2, w2, vl);
  t = __riscv_vadd_vv_u16m2(t, w1, vl);
  t = __riscv_vadd_vv_u16m2(t, w0, vl);
  t = __riscv_vadd_vv_u16m2(t, v0, vl);
  t = __riscv_vsrl_vx_u16m2(__riscv_vadd_vx_u16m2(t, 4, vl), 3, vl);
  *op2 = __riscv_vncvt_x_x_w_u8m1(t, vl);

  t = __riscv_vmul_vx_u16m2(w3, 2, vl);
  t = __riscv_vadd_vv_u16m2(t, w2, vl);
  t = __riscv_vmacc_vx_u16m2(t, 2, w1, vl);
  t = __riscv_vadd_vv_u16m2(t, w0, vl);
  t = __riscv_vadd_vv_u16m2(t, v0, vl);
  t = __riscv_vadd_vv_u16m2(t, v1, vl);
  t = __riscv_vsrl_vx_u16m2(__riscv_vadd_vx_u16m2(t, 4, vl), 3, vl);
  *op1 = __riscv_vncvt_x_x_w_u8m1(t, vl);

  t = __riscv_vadd_vv_u16m2(w3, w2, vl);
  t = __riscv_vadd_vv_u16m2(t, w1, vl);
  t = __riscv_vmacc_vx_u16m2(t, 2, w0, vl);
  t = __riscv_vadd_vv_u16m2(t, v0, vl);
  t = __riscv_vadd_vv_u16m2(t, v1, vl);
  t = __riscv_vadd_vv_u16m2(t, v2, vl);
  t = __riscv_vsrl_vx_u16m2(__riscv_vadd_vx_u16m2(t, 4, vl), 3, vl);
  *op0 = __riscv_vncvt_x_x_w_u8m1(t, vl);

  t = __riscv_vadd_vv_u16m2(w2, w1, vl);
  t = __riscv_vadd_vv_u16m2(t, w0, vl);
  t = __riscv_vmacc_vx_u16m2(t, 2, v0, vl);
  t = __riscv_vadd_vv_u16m2(t, v1, vl);
  t = __riscv_vadd_vv_u16m2(t, v2, vl);
  t = __riscv_vadd_vv_u16m2(t, v3, vl);
  t = __riscv_vsrl_vx_u16m2(__riscv_vadd_vx_u16m2(t, 4, vl), 3, vl);
  *oq0 = __riscv_vncvt_x_x_w_u8m1(t, vl);

  t = __riscv_vadd_vv_u16m2(w1, w0, vl);
  t = __riscv_vadd_vv_u16m2(t, v0, vl);
  t = __riscv_vmacc_vx_u16m2(t, 2, v1, vl);
  t = __riscv_vadd_vv_u16m2(t, v2, vl);
  t = __riscv_vmacc_vx_u16m2(t, 2, v3, vl);
  t = __riscv_vsrl_vx_u16m2(__riscv_vadd_vx_u16m2(t, 4, vl), 3, vl);
  *oq1 = __riscv_vncvt_x_x_w_u8m1(t, vl);

  t = __riscv_vadd_vv_u16m2(w0, v0, vl);
  t = __riscv_vadd_vv_u16m2(t, v1, vl);
  t = __riscv_vmacc_vx_u16m2(t, 2, v2, vl);
  t = __riscv_vadd_vv_u16m2(t, v3, vl);
  t = __riscv_vmacc_vx_u16m2(t, 3, v3, vl);
  t = __riscv_vsrl_vx_u16m2(__riscv_vadd_vx_u16m2(t, 4, vl), 3, vl);
  *oq2 = __riscv_vncvt_x_x_w_u8m1(t, vl);
}


/* Shared mask computation with vector parameters (supports dual: 16 cols
 * with per-8-col thresholds packed into the limit/blimit/thresh vectors). */
static INLINE vbool8_t lpf_filter_mask(vuint8m1_t p3, vuint8m1_t p2,
                                       vuint8m1_t p1, vuint8m1_t p0,
                                       vuint8m1_t q0, vuint8m1_t q1,
                                       vuint8m1_t q2, vuint8m1_t q3,
                                       vuint8m1_t limit, vuint8m1_t blimit,
                                       size_t vl) {
  vbool8_t m = __riscv_vmsgtu_vv_u8m1_b8(lpf_absdiff_u8(p3, p2, vl), limit, vl);
  m = __riscv_vmor_mm_b8(
      m, __riscv_vmsgtu_vv_u8m1_b8(lpf_absdiff_u8(p2, p1, vl), limit, vl), vl);
  m = __riscv_vmor_mm_b8(
      m, __riscv_vmsgtu_vv_u8m1_b8(lpf_absdiff_u8(p1, p0, vl), limit, vl), vl);
  m = __riscv_vmor_mm_b8(
      m, __riscv_vmsgtu_vv_u8m1_b8(lpf_absdiff_u8(q1, q0, vl), limit, vl), vl);
  m = __riscv_vmor_mm_b8(
      m, __riscv_vmsgtu_vv_u8m1_b8(lpf_absdiff_u8(q2, q1, vl), limit, vl), vl);
  m = __riscv_vmor_mm_b8(
      m, __riscv_vmsgtu_vv_u8m1_b8(lpf_absdiff_u8(q3, q2, vl), limit, vl), vl);
  const vuint16m2_t a0 = __riscv_vzext_vf2_u16m2(lpf_absdiff_u8(p0, q0, vl), vl);
  const vuint16m2_t a1 = __riscv_vzext_vf2_u16m2(lpf_absdiff_u8(p1, q1, vl), vl);
  const vuint16m2_t sum = __riscv_vadd_vv_u16m2(
      __riscv_vadd_vv_u16m2(a0, a0, vl), __riscv_vsrl_vx_u16m2(a1, 1, vl),
      vl);
  const vuint16m2_t bl16 = __riscv_vzext_vf2_u16m2(blimit, vl);
  m = __riscv_vmor_mm_b8(m, __riscv_vmsgtu_vv_u16m2_b8(sum, bl16, vl), vl);
  return __riscv_vmnot_m_b8(m, vl);
}

static INLINE vbool8_t lpf_flat_mask4(vuint8m1_t p3, vuint8m1_t p2,
                                      vuint8m1_t p1, vuint8m1_t p0,
                                      vuint8m1_t q0, vuint8m1_t q1,
                                      vuint8m1_t q2, vuint8m1_t q3,
                                      size_t vl) {
  vbool8_t fm =
      __riscv_vmsgtu_vx_u8m1_b8(lpf_absdiff_u8(p1, p0, vl), 1, vl);
  fm = __riscv_vmor_mm_b8(
      fm, __riscv_vmsgtu_vx_u8m1_b8(lpf_absdiff_u8(q1, q0, vl), 1, vl), vl);
  fm = __riscv_vmor_mm_b8(
      fm, __riscv_vmsgtu_vx_u8m1_b8(lpf_absdiff_u8(p2, p0, vl), 1, vl), vl);
  fm = __riscv_vmor_mm_b8(
      fm, __riscv_vmsgtu_vx_u8m1_b8(lpf_absdiff_u8(q2, q0, vl), 1, vl), vl);
  fm = __riscv_vmor_mm_b8(
      fm, __riscv_vmsgtu_vx_u8m1_b8(lpf_absdiff_u8(p3, p0, vl), 1, vl), vl);
  fm = __riscv_vmor_mm_b8(
      fm, __riscv_vmsgtu_vx_u8m1_b8(lpf_absdiff_u8(q3, q0, vl), 1, vl), vl);
  return __riscv_vmnot_m_b8(fm, vl);
}

static INLINE vbool8_t lpf_hev_mask(vuint8m1_t p1, vuint8m1_t p0,
                                    vuint8m1_t q0, vuint8m1_t q1,
                                    vuint8m1_t thresh, size_t vl) {
  vbool8_t hev =
      __riscv_vmsgtu_vv_u8m1_b8(lpf_absdiff_u8(p1, p0, vl), thresh, vl);
  hev = __riscv_vmor_mm_b8(
      hev, __riscv_vmsgtu_vv_u8m1_b8(lpf_absdiff_u8(q1, q0, vl), thresh, vl),
      vl);
  return hev;
}

static INLINE vuint8m1_t lpf_pack2(vuint8m1_t lo, vuint8m1_t hi, size_t vl) {
  /* first vl/2 lanes from lo, rest from hi */
  const vuint8m1_t vid = __riscv_vid_v_u8m1(vl);
  const vbool8_t sel =
      __riscv_vmsltu_vx_u8m1_b8(vid, (unsigned)(vl / 2), vl);
  return __riscv_vmerge_vvm_u8m1(hi, lo, sel, vl);
}

void vpx_lpf_horizontal_8_rvv(uint8_t *s, int pitch, const uint8_t *blimit,
                              const uint8_t *limit, const uint8_t *thresh) {
  const size_t vl = LPF_VL;
  const vuint8m1_t p3 = __riscv_vle8_v_u8m1(s - 4 * pitch, vl);
  const vuint8m1_t p2 = __riscv_vle8_v_u8m1(s - 3 * pitch, vl);
  const vuint8m1_t p1 = __riscv_vle8_v_u8m1(s - 2 * pitch, vl);
  const vuint8m1_t p0 = __riscv_vle8_v_u8m1(s - pitch, vl);
  const vuint8m1_t q0 = __riscv_vle8_v_u8m1(s, vl);
  const vuint8m1_t q1 = __riscv_vle8_v_u8m1(s + pitch, vl);
  const vuint8m1_t q2 = __riscv_vle8_v_u8m1(s + 2 * pitch, vl);
  const vuint8m1_t q3 = __riscv_vle8_v_u8m1(s + 3 * pitch, vl);

  const vuint8m1_t lim = __riscv_vmv_v_x_u8m1(*limit, vl);
  const vuint8m1_t bl = __riscv_vmv_v_x_u8m1(*blimit, vl);
  const vuint8m1_t th = __riscv_vmv_v_x_u8m1(*thresh, vl);
  const vbool8_t mask_b = lpf_filter_mask(p3, p2, p1, p0, q0, q1, q2, q3, lim,
                                          bl, vl);
  const vbool8_t flat_b = lpf_flat_mask4(p3, p2, p1, p0, q0, q1, q2, q3, vl);
  const vbool8_t hev_b = lpf_hev_mask(p1, p0, q0, q1, th, vl);

  vuint8m1_t f4_op1, f4_op0, f4_oq0, f4_oq1;
  lpf_filter4(mask_b, hev_b, p1, p0, q0, q1, vl, &f4_op1, &f4_op0, &f4_oq0,
              &f4_oq1);

  vuint8m1_t f8_op2, f8_op1, f8_op0, f8_oq0, f8_oq1, f8_oq2;
  lpf_filter8_taps(p3, p2, p1, p0, q0, q1, q2, q3, vl, &f8_op2, &f8_op1,
                   &f8_op0, &f8_oq0, &f8_oq1, &f8_oq2);

  const vbool8_t use8 = __riscv_vmand_mm_b8(flat_b, mask_b, vl);

  {
    vuint8m1_t o;
    o = __riscv_vmerge_vvm_u8m1(p2, f8_op2, use8, vl);
    __riscv_vse8_v_u8m1(s - 3 * pitch, o, vl);
    o = __riscv_vmerge_vvm_u8m1(f4_op1, f8_op1, use8, vl);
    __riscv_vse8_v_u8m1(s - 2 * pitch, o, vl);
    o = __riscv_vmerge_vvm_u8m1(f4_op0, f8_op0, use8, vl);
    __riscv_vse8_v_u8m1(s - pitch, o, vl);
    o = __riscv_vmerge_vvm_u8m1(f4_oq0, f8_oq0, use8, vl);
    __riscv_vse8_v_u8m1(s, o, vl);
    o = __riscv_vmerge_vvm_u8m1(f4_oq1, f8_oq1, use8, vl);
    __riscv_vse8_v_u8m1(s + pitch, o, vl);
    o = __riscv_vmerge_vvm_u8m1(q2, f8_oq2, use8, vl);
    __riscv_vse8_v_u8m1(s + 2 * pitch, o, vl);
  }
}

void vpx_lpf_horizontal_4_rvv(uint8_t *s, int pitch, const uint8_t *blimit,
                              const uint8_t *limit, const uint8_t *thresh) {
  const size_t vl = LPF_VL;
  const vuint8m1_t p3 = __riscv_vle8_v_u8m1(s - 4 * pitch, vl);
  const vuint8m1_t p2 = __riscv_vle8_v_u8m1(s - 3 * pitch, vl);
  const vuint8m1_t p1 = __riscv_vle8_v_u8m1(s - 2 * pitch, vl);
  const vuint8m1_t p0 = __riscv_vle8_v_u8m1(s - pitch, vl);
  const vuint8m1_t q0 = __riscv_vle8_v_u8m1(s, vl);
  const vuint8m1_t q1 = __riscv_vle8_v_u8m1(s + pitch, vl);
  const vuint8m1_t q2 = __riscv_vle8_v_u8m1(s + 2 * pitch, vl);
  const vuint8m1_t q3 = __riscv_vle8_v_u8m1(s + 3 * pitch, vl);

  const vuint8m1_t lim = __riscv_vmv_v_x_u8m1(*limit, vl);
  const vuint8m1_t bl = __riscv_vmv_v_x_u8m1(*blimit, vl);
  const vuint8m1_t th = __riscv_vmv_v_x_u8m1(*thresh, vl);
  const vbool8_t mask_b = lpf_filter_mask(p3, p2, p1, p0, q0, q1, q2, q3, lim,
                                          bl, vl);
  const vbool8_t hev_b = lpf_hev_mask(p1, p0, q0, q1, th, vl);

  vuint8m1_t op1, op0, oq0, oq1;
  lpf_filter4(mask_b, hev_b, p1, p0, q0, q1, vl, &op1, &op0, &oq0, &oq1);

  __riscv_vse8_v_u8m1(s - 2 * pitch, op1, vl);
  __riscv_vse8_v_u8m1(s - pitch, op0, vl);
  __riscv_vse8_v_u8m1(s, oq0, vl);
  __riscv_vse8_v_u8m1(s + pitch, oq1, vl);
}

/* Dual variants: 16 columns in one pass (vl=16), first 8 columns use
 * the *0 thresholds, last 8 the *1 thresholds. Bit-exact vs the C dual
 * functions which call the single-column-loop C kernels twice. */

void vpx_lpf_horizontal_8_dual_rvv(uint8_t *s, int pitch,
                                   const uint8_t *blimit0,
                                   const uint8_t *limit0,
                                   const uint8_t *thresh0,
                                   const uint8_t *blimit1,
                                   const uint8_t *limit1,
                                   const uint8_t *thresh1) {
  const size_t vl = 2 * LPF_VL;
  const vuint8m1_t p3 = __riscv_vle8_v_u8m1(s - 4 * pitch, vl);
  const vuint8m1_t p2 = __riscv_vle8_v_u8m1(s - 3 * pitch, vl);
  const vuint8m1_t p1 = __riscv_vle8_v_u8m1(s - 2 * pitch, vl);
  const vuint8m1_t p0 = __riscv_vle8_v_u8m1(s - pitch, vl);
  const vuint8m1_t q0 = __riscv_vle8_v_u8m1(s, vl);
  const vuint8m1_t q1 = __riscv_vle8_v_u8m1(s + pitch, vl);
  const vuint8m1_t q2 = __riscv_vle8_v_u8m1(s + 2 * pitch, vl);
  const vuint8m1_t q3 = __riscv_vle8_v_u8m1(s + 3 * pitch, vl);

  const vuint8m1_t lim = lpf_pack2(__riscv_vmv_v_x_u8m1(*limit0, vl),
                                   __riscv_vmv_v_x_u8m1(*limit1, vl), vl);
  const vuint8m1_t bl = lpf_pack2(__riscv_vmv_v_x_u8m1(*blimit0, vl),
                                  __riscv_vmv_v_x_u8m1(*blimit1, vl), vl);
  const vuint8m1_t th = lpf_pack2(__riscv_vmv_v_x_u8m1(*thresh0, vl),
                                  __riscv_vmv_v_x_u8m1(*thresh1, vl), vl);
  const vbool8_t mask_b =
      lpf_filter_mask(p3, p2, p1, p0, q0, q1, q2, q3, lim, bl, vl);
  const vbool8_t flat_b = lpf_flat_mask4(p3, p2, p1, p0, q0, q1, q2, q3, vl);
  const vbool8_t hev_b = lpf_hev_mask(p1, p0, q0, q1, th, vl);

  vuint8m1_t f4_op1, f4_op0, f4_oq0, f4_oq1;
  lpf_filter4(mask_b, hev_b, p1, p0, q0, q1, vl, &f4_op1, &f4_op0, &f4_oq0,
              &f4_oq1);

  vuint8m1_t f8_op2, f8_op1, f8_op0, f8_oq0, f8_oq1, f8_oq2;
  lpf_filter8_taps(p3, p2, p1, p0, q0, q1, q2, q3, vl, &f8_op2, &f8_op1,
                   &f8_op0, &f8_oq0, &f8_oq1, &f8_oq2);

  const vbool8_t use8 = __riscv_vmand_mm_b8(flat_b, mask_b, vl);

  vuint8m1_t o;
  o = __riscv_vmerge_vvm_u8m1(p2, f8_op2, use8, vl);
  __riscv_vse8_v_u8m1(s - 3 * pitch, o, vl);
  o = __riscv_vmerge_vvm_u8m1(f4_op1, f8_op1, use8, vl);
  __riscv_vse8_v_u8m1(s - 2 * pitch, o, vl);
  o = __riscv_vmerge_vvm_u8m1(f4_op0, f8_op0, use8, vl);
  __riscv_vse8_v_u8m1(s - pitch, o, vl);
  o = __riscv_vmerge_vvm_u8m1(f4_oq0, f8_oq0, use8, vl);
  __riscv_vse8_v_u8m1(s, o, vl);
  o = __riscv_vmerge_vvm_u8m1(f4_oq1, f8_oq1, use8, vl);
  __riscv_vse8_v_u8m1(s + pitch, o, vl);
  o = __riscv_vmerge_vvm_u8m1(q2, f8_oq2, use8, vl);
  __riscv_vse8_v_u8m1(s + 2 * pitch, o, vl);
}

void vpx_lpf_horizontal_4_dual_rvv(uint8_t *s, int pitch,
                                   const uint8_t *blimit0,
                                   const uint8_t *limit0,
                                   const uint8_t *thresh0,
                                   const uint8_t *blimit1,
                                   const uint8_t *limit1,
                                   const uint8_t *thresh1) {
  const size_t vl = 2 * LPF_VL;
  const vuint8m1_t p3 = __riscv_vle8_v_u8m1(s - 4 * pitch, vl);
  const vuint8m1_t p2 = __riscv_vle8_v_u8m1(s - 3 * pitch, vl);
  const vuint8m1_t p1 = __riscv_vle8_v_u8m1(s - 2 * pitch, vl);
  const vuint8m1_t p0 = __riscv_vle8_v_u8m1(s - pitch, vl);
  const vuint8m1_t q0 = __riscv_vle8_v_u8m1(s, vl);
  const vuint8m1_t q1 = __riscv_vle8_v_u8m1(s + pitch, vl);
  const vuint8m1_t q2 = __riscv_vle8_v_u8m1(s + 2 * pitch, vl);
  const vuint8m1_t q3 = __riscv_vle8_v_u8m1(s + 3 * pitch, vl);

  const vuint8m1_t lim = lpf_pack2(__riscv_vmv_v_x_u8m1(*limit0, vl),
                                   __riscv_vmv_v_x_u8m1(*limit1, vl), vl);
  const vuint8m1_t bl = lpf_pack2(__riscv_vmv_v_x_u8m1(*blimit0, vl),
                                  __riscv_vmv_v_x_u8m1(*blimit1, vl), vl);
  const vuint8m1_t th = lpf_pack2(__riscv_vmv_v_x_u8m1(*thresh0, vl),
                                  __riscv_vmv_v_x_u8m1(*thresh1, vl), vl);
  const vbool8_t mask_b =
      lpf_filter_mask(p3, p2, p1, p0, q0, q1, q2, q3, lim, bl, vl);
  const vbool8_t hev_b = lpf_hev_mask(p1, p0, q0, q1, th, vl);

  vuint8m1_t op1, op0, oq0, oq1;
  lpf_filter4(mask_b, hev_b, p1, p0, q0, q1, vl, &op1, &op0, &oq0, &oq1);

  __riscv_vse8_v_u8m1(s - 2 * pitch, op1, vl);
  __riscv_vse8_v_u8m1(s - pitch, op0, vl);
  __riscv_vse8_v_u8m1(s, oq0, vl);
  __riscv_vse8_v_u8m1(s + pitch, oq1, vl);
}
