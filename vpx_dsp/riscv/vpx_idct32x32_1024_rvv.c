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
#include "vpx_dsp/inv_txfm.h"
#include "vpx_dsp/txfm_common.h"

/* RVV 32x32 inverse DCT (full 1024-coefficient variant).
 *
 * Bit-exact vs vpx_idct32x32_1024_add_c: both row and column passes run
 * idct32 on 16 lanes in parallel (VLEN=256, e16m1/e32m2). Each vector lane
 * is one row (row pass, strided loads) or one column (column pass, unit
 * loads); the 32-point butterfly is fully inter-register.
 *
 * Precision contract (must match the C scalar code exactly):
 *  - constant butterflies multiply in the int domain and round-shift by
 *    DCT_CONST_BITS before WRAPLOW (truncate to i16): done in i32 here,
 *    truncation via vncvt (mod 2^16 == (int16_t) cast).
 *  - WRAPLOW(a+b) truncates the int-domain sum: plain i16 vadd/sub wrap
 *    identically, so those stay in i16.
 *  - (a+b)*c sums in the int domain (untruncated) before multiplying:
 *    widen first, add, then multiply.
 */

#define VL 16
typedef __attribute__((riscv_rvv_vector_bits(256))) vint16m1_t V;
typedef __attribute__((riscv_rvv_vector_bits(512))) vint32m2_t W;

#define EXT(x) __riscv_vsext_vf2_i32m2((x), VL)
#define NARROW(x) __riscv_vncvt_x_x_w_i16m1((x), VL)
#define RSHIFT(x) \
  __riscv_vsra_vx_i32m2( \
      __riscv_vadd_vx_i32m2((x), DCT_CONST_ROUNDING, VL), DCT_CONST_BITS, VL)

/* oa = WRAP(a*c1 - b*c2); ob = WRAP(a*c2 + b*c1) */
#define CB(a, b, c1, c2, oa, ob)                    \
  do {                                              \
    vint32m2_t mta = __riscv_vmul_vx_i32m2(EXT(a), (c1), VL); \
    mta = __riscv_vsub_vv_i32m2( \
        mta, __riscv_vmul_vx_i32m2(EXT(b), (c2), VL), VL);     \
    (oa) = NARROW(RSHIFT(mta));                  \
    vint32m2_t mtb = __riscv_vmul_vx_i32m2(EXT(a), (c2), VL); \
    mtb = __riscv_vadd_vv_i32m2( \
        mtb, __riscv_vmul_vx_i32m2(EXT(b), (c1), VL), VL);     \
    (ob) = NARROW(RSHIFT(mtb));                  \
  } while (0)

/* oa = WRAP(a + b); ob = WRAP(a - b) (i16 wrap == truncation) */
#define AB(a, b, oa, ob)                                       \
  do {                                                         \
    (oa) = __riscv_vadd_vv_i16m1((a), (b), VL);                \
    (ob) = __riscv_vsub_vv_i16m1((a), (b), VL);                \
  } while (0)

/* oa = WRAP((a + b) * c) — sum in int domain */
#define MADD(a, b, c, oa)                                            \
  do {                                                               \
    vint32m2_t mp = __riscv_vadd_vv_i32m2(EXT(a), EXT(b), VL);        \
    mp = __riscv_vmul_vx_i32m2(mp, (c), VL);                           \
    (oa) = NARROW(RSHIFT(mp));                                        \
  } while (0)

/* oa = WRAP((a - b) * c) */
#define MSUB(a, b, c, oa)                                            \
  do {                                                               \
    vint32m2_t mp = __riscv_vsub_vv_i32m2(EXT(a), EXT(b), VL);        \
    mp = __riscv_vmul_vx_i32m2(mp, (c), VL);                           \
    (oa) = NARROW(RSHIFT(mp));                                        \
  } while (0)

/* oa = WRAP((-a + b) * c) == WRAP((b - a) * c) */
#define MADDN(a, b, c, oa) MSUB(b, a, c, oa)

/* oa = WRAP(-(a*c1 + b*c2)) — negate before rounding shift */
#define NEGSUM(a, b, c1, c2, oa)                                    \
  do {                                                              \
    vint32m2_t mp = __riscv_vmul_vx_i32m2(EXT(a), (c1), VL);         \
    mp = __riscv_vadd_vv_i32m2(                                      \
        mp, __riscv_vmul_vx_i32m2(EXT(b), (c2), VL), VL);            \
    mp = __riscv_vrsub_vx_i32m2(mp, 0, VL);                           \
    (oa) = NARROW(RSHIFT(mp));                                       \
  } while (0)

/* oa = WRAP(-a*c1 + b*c2) — no rounding sum, direct products */
#define CBN1(a, b, c1, c2, oa)                                  \
  do {                                                          \
    vint32m2_t mp = __riscv_vmul_vx_i32m2(EXT(b), (c2), VL);     \
    mp = __riscv_vsub_vv_i32m2( \
        mp, __riscv_vmul_vx_i32m2(EXT(a), (c1), VL), VL);        \
    (oa) = NARROW(RSHIFT(mp));                                   \
  } while (0)

static void idct32_vec(V s[32]) {
  V t[32];

  /* stage 1 (matches C input permutation + pair butterflies) */
  {
    V p[32];
    p[0] = s[0];  p[1] = s[16]; p[2] = s[8];  p[3] = s[24];
    p[4] = s[4];  p[5] = s[20]; p[6] = s[12]; p[7] = s[28];
    p[8] = s[2];  p[9] = s[18]; p[10] = s[10]; p[11] = s[26];
    p[12] = s[6]; p[13] = s[22]; p[14] = s[14]; p[15] = s[30];
    CB(s[1], s[31], cospi_31_64, cospi_1_64, t[16], t[31]);
    CB(s[17], s[15], cospi_15_64, cospi_17_64, t[17], t[30]);
    CB(s[9], s[23], cospi_23_64, cospi_9_64, t[18], t[29]);
    CB(s[25], s[7], cospi_7_64, cospi_25_64, t[19], t[28]);
    CB(s[5], s[27], cospi_27_64, cospi_5_64, t[20], t[27]);
    CB(s[21], s[11], cospi_11_64, cospi_21_64, t[21], t[26]);
    CB(s[13], s[19], cospi_19_64, cospi_13_64, t[22], t[25]);
    CB(s[29], s[3], cospi_3_64, cospi_29_64, t[23], t[24]);
    for (int i = 0; i < 16; ++i) t[i] = p[i];
  }

  /* stage 2 */
  {
    V p[32];
    for (int i = 0; i < 8; ++i) p[i] = t[i];
    CB(t[8], t[15], cospi_30_64, cospi_2_64, p[8], p[15]);
    CB(t[9], t[14], cospi_14_64, cospi_18_64, p[9], p[14]);
    CB(t[10], t[13], cospi_22_64, cospi_10_64, p[10], p[13]);
    CB(t[11], t[12], cospi_6_64, cospi_26_64, p[11], p[12]);
    AB(t[16], t[17], p[16], p[17]);
    AB(t[19], t[18], p[19], p[18]);   /* p18 = -t18+t19, p19 = t18+t19 */
    AB(t[20], t[21], p[20], p[21]);
    AB(t[23], t[22], p[23], p[22]);
    AB(t[24], t[25], p[24], p[25]);
    AB(t[27], t[26], p[27], p[26]);
    AB(t[28], t[29], p[28], p[29]);
    AB(t[31], t[30], p[31], p[30]);
    for (int i = 0; i < 32; ++i) t[i] = p[i];
  }

  /* stage 3 */
  {
    V p[32];
    for (int i = 0; i < 4; ++i) p[i] = t[i];
    CB(t[4], t[7], cospi_28_64, cospi_4_64, p[4], p[7]);
    CB(t[5], t[6], cospi_12_64, cospi_20_64, p[5], p[6]);
    AB(t[8], t[9], p[8], p[9]);
    AB(t[11], t[10], p[11], p[10]);
    AB(t[12], t[13], p[12], p[13]);
    AB(t[15], t[14], p[15], p[14]);
    p[16] = t[16]; p[31] = t[31];
    CBN1(t[17], t[30], cospi_4_64, cospi_28_64, p[17]);
    {
      /* p30 = WRAP(t17*c28 + t30*c4) */
      vint32m2_t mtb = __riscv_vmul_vx_i32m2(EXT(t[17]), cospi_28_64, VL);
      mtb = __riscv_vadd_vv_i32m2(
          mtb, __riscv_vmul_vx_i32m2(EXT(t[30]), cospi_4_64, VL), VL);
      p[30] = NARROW(RSHIFT(mtb));
    }
    NEGSUM(t[18], t[29], cospi_28_64, cospi_4_64, p[18]);
    CBN1(t[18], t[29], cospi_4_64, cospi_28_64, p[29]);
    p[19] = t[19]; p[20] = t[20];
    CBN1(t[21], t[26], cospi_20_64, cospi_12_64, p[21]);
    {
      /* p26 = WRAP(t21*c12 + t26*c20) */
      vint32m2_t mtb = __riscv_vmul_vx_i32m2(EXT(t[21]), cospi_12_64, VL);
      mtb = __riscv_vadd_vv_i32m2(
          mtb, __riscv_vmul_vx_i32m2(EXT(t[26]), cospi_20_64, VL), VL);
      p[26] = NARROW(RSHIFT(mtb));
    }
    NEGSUM(t[22], t[25], cospi_12_64, cospi_20_64, p[22]);
    CBN1(t[22], t[25], cospi_20_64, cospi_12_64, p[25]);
    p[23] = t[23]; p[24] = t[24]; p[27] = t[27]; p[28] = t[28];
    for (int i = 0; i < 32; ++i) t[i] = p[i];
  }

  /* stage 4 */
  {
    V p[32];
    MADD(t[0], t[1], cospi_16_64, p[0]);
    MSUB(t[0], t[1], cospi_16_64, p[1]);
    CB(t[2], t[3], cospi_24_64, cospi_8_64, p[2], p[3]);
    AB(t[4], t[5], p[4], p[5]);
    AB(t[7], t[6], p[7], p[6]);
    p[8] = t[8]; p[15] = t[15];
    CBN1(t[9], t[14], cospi_8_64, cospi_24_64, p[9]);
    {
      vint32m2_t mtb = __riscv_vmul_vx_i32m2(EXT(t[9]), cospi_24_64, VL);
      mtb = __riscv_vadd_vv_i32m2(
          mtb, __riscv_vmul_vx_i32m2(EXT(t[14]), cospi_8_64, VL), VL);
      p[14] = NARROW(RSHIFT(mtb));
    }
    NEGSUM(t[10], t[13], cospi_24_64, cospi_8_64, p[10]);
    {
      vint32m2_t mtb = __riscv_vmul_vx_i32m2(EXT(t[13]), cospi_24_64, VL);
      mtb = __riscv_vsub_vv_i32m2(
          mtb, __riscv_vmul_vx_i32m2(EXT(t[10]), cospi_8_64, VL), VL);
      p[13] = NARROW(RSHIFT(mtb));
    }
    p[11] = t[11]; p[12] = t[12];
    AB(t[16], t[19], p[16], p[19]);
    AB(t[17], t[18], p[17], p[18]);
    AB(t[23], t[20], p[23], p[20]);
    AB(t[22], t[21], p[22], p[21]);
    AB(t[24], t[27], p[24], p[27]);
    AB(t[25], t[26], p[25], p[26]);
    AB(t[31], t[28], p[31], p[28]);
    AB(t[30], t[29], p[30], p[29]);
    for (int i = 0; i < 32; ++i) t[i] = p[i];
  }

  /* stage 5 */
  {
    V p[32];
    AB(t[0], t[3], p[0], p[3]);
    AB(t[1], t[2], p[1], p[2]);
    p[4] = t[4];
    MADDN(t[5], t[6], cospi_16_64, p[5]);   /* (t6 - t5) * c16 */
    MADD(t[5], t[6], cospi_16_64, p[6]);    /* (t5 + t6) * c16 */
    p[7] = t[7];
    AB(t[8], t[11], p[8], p[11]);
    AB(t[9], t[10], p[9], p[10]);
    AB(t[15], t[12], p[15], p[12]);
    AB(t[14], t[13], p[14], p[13]);
    p[16] = t[16]; p[17] = t[17];
    CBN1(t[18], t[29], cospi_8_64, cospi_24_64, p[18]);
    {
      vint32m2_t mtb = __riscv_vmul_vx_i32m2(EXT(t[18]), cospi_24_64, VL);
      mtb = __riscv_vadd_vv_i32m2(
          mtb, __riscv_vmul_vx_i32m2(EXT(t[29]), cospi_8_64, VL), VL);
      p[29] = NARROW(RSHIFT(mtb));
    }
    CBN1(t[19], t[28], cospi_8_64, cospi_24_64, p[19]);
    {
      vint32m2_t mtb = __riscv_vmul_vx_i32m2(EXT(t[19]), cospi_24_64, VL);
      mtb = __riscv_vadd_vv_i32m2(
          mtb, __riscv_vmul_vx_i32m2(EXT(t[28]), cospi_8_64, VL), VL);
      p[28] = NARROW(RSHIFT(mtb));
    }
    NEGSUM(t[20], t[27], cospi_24_64, cospi_8_64, p[20]);
    {
      vint32m2_t mtb = __riscv_vmul_vx_i32m2(EXT(t[27]), cospi_24_64, VL);
      mtb = __riscv_vsub_vv_i32m2(
          mtb, __riscv_vmul_vx_i32m2(EXT(t[20]), cospi_8_64, VL), VL);
      p[27] = NARROW(RSHIFT(mtb));
    }
    NEGSUM(t[21], t[26], cospi_24_64, cospi_8_64, p[21]);
    {
      vint32m2_t mtb = __riscv_vmul_vx_i32m2(EXT(t[26]), cospi_24_64, VL);
      mtb = __riscv_vsub_vv_i32m2(
          mtb, __riscv_vmul_vx_i32m2(EXT(t[21]), cospi_8_64, VL), VL);
      p[26] = NARROW(RSHIFT(mtb));
    }
    p[22] = t[22]; p[23] = t[23]; p[24] = t[24]; p[25] = t[25];
    p[30] = t[30]; p[31] = t[31];
    for (int i = 0; i < 32; ++i) t[i] = p[i];
  }

  /* stage 6 */
  {
    V p[32];
    AB(t[0], t[7], p[0], p[7]);
    AB(t[1], t[6], p[1], p[6]);
    AB(t[2], t[5], p[2], p[5]);
    AB(t[3], t[4], p[3], p[4]);
    p[8] = t[8]; p[9] = t[9];
    MADDN(t[10], t[13], cospi_16_64, p[10]);
    MADD(t[10], t[13], cospi_16_64, p[13]);
    MADDN(t[11], t[12], cospi_16_64, p[11]);
    MADD(t[11], t[12], cospi_16_64, p[12]);
    p[14] = t[14]; p[15] = t[15];
    AB(t[16], t[23], p[16], p[23]);
    AB(t[17], t[22], p[17], p[22]);
    AB(t[18], t[21], p[18], p[21]);
    AB(t[19], t[20], p[19], p[20]);
    AB(t[31], t[24], p[31], p[24]);
    AB(t[30], t[25], p[30], p[25]);
    AB(t[29], t[26], p[29], p[26]);
    AB(t[28], t[27], p[28], p[27]);
    for (int i = 0; i < 32; ++i) t[i] = p[i];
  }

  /* stage 7 */
  {
    V p[32];
    AB(t[0], t[15], p[0], p[15]);
    AB(t[1], t[14], p[1], p[14]);
    AB(t[2], t[13], p[2], p[13]);
    AB(t[3], t[12], p[3], p[12]);
    AB(t[4], t[11], p[4], p[11]);
    AB(t[5], t[10], p[5], p[10]);
    AB(t[6], t[9], p[6], p[9]);
    AB(t[7], t[8], p[7], p[8]);
    p[16] = t[16]; p[17] = t[17]; p[18] = t[18]; p[19] = t[19];
    MADDN(t[20], t[27], cospi_16_64, p[20]);
    MADD(t[20], t[27], cospi_16_64, p[27]);
    MADDN(t[21], t[26], cospi_16_64, p[21]);
    MADD(t[21], t[26], cospi_16_64, p[26]);
    MADDN(t[22], t[25], cospi_16_64, p[22]);
    MADD(t[22], t[25], cospi_16_64, p[25]);
    MADDN(t[23], t[24], cospi_16_64, p[23]);
    MADD(t[23], t[24], cospi_16_64, p[24]);
    p[28] = t[28]; p[29] = t[29]; p[30] = t[30]; p[31] = t[31];
    for (int i = 0; i < 32; ++i) t[i] = p[i];
  }

  /* final: output[i] = WRAP(step1[i] + step1[31-i]) / WRAP(step1[15-i] - step1[16+i]) */
  {
    /* out low half: t[i] + t[31-i]; high half: t[15-i] - t[16+i] */
    V o[32];
    for (int i = 0; i < 16; ++i) {
      o[i] = __riscv_vadd_vv_i16m1(t[i], t[31 - i], VL);
      o[16 + i] = __riscv_vsub_vv_i16m1(t[15 - i], t[16 + i], VL);
    }
    for (int i = 0; i < 32; ++i) s[i] = o[i];
  }
}

void vpx_idct32x32_1024_add_rvv(const tran_low_t *input, uint8_t *dest,
                                int stride) {
  tran_low_t out[32 * 32];

  /* Rows: two blocks of 16 lanes. */
  for (int blk = 0; blk < 32; blk += VL) {
    V s[32];
    for (int k = 0; k < 32; ++k) {
      s[k] = __riscv_vlse16_v_i16m1(input + blk * 32 + k, 64, VL);
    }
    idct32_vec(s);
    for (int k = 0; k < 32; ++k) {
      __riscv_vsse16_v_i16m1(out + blk * 32 + k, 64, s[k], VL);
    }
  }

  /* Columns: two blocks of 16 lanes, add to dest. */
  for (int blk = 0; blk < 32; blk += VL) {
    V s[32];
    for (int j = 0; j < 32; ++j) {
      s[j] = __riscv_vle16_v_i16m1(out + j * 32 + blk, VL);
    }
    idct32_vec(s);
    for (int j = 0; j < 32; ++j) {
      vint32m2_t v = EXT(s[j]);
      v = __riscv_vsra_vx_i32m2(__riscv_vadd_vx_i32m2(v, 32, VL), 6, VL);
      const vuint8mf2_t d = __riscv_vle8_v_u8mf2(dest + j * stride + blk, VL);
      vint32m2_t dv =
          __riscv_vreinterpret_v_u32m2_i32m2(__riscv_vzext_vf4_u32m2(d, VL));
      dv = __riscv_vadd_vv_i32m2(dv, v, VL);
      dv = __riscv_vmax_vx_i32m2(dv, 0, VL);
      dv = __riscv_vmin_vx_i32m2(dv, 255, VL);
      const vuint16m1_t mid =
          __riscv_vreinterpret_v_i16m1_u16m1(NARROW(dv));
      const vuint8mf2_t r = __riscv_vncvt_x_x_w_u8mf2(mid, VL);
      __riscv_vse8_v_u8mf2(dest + j * stride + blk, r, VL);
    }
  }
}
