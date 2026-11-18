/* libvpx RVV: SAD kernel (ported from open-risc-v/libvpx-rvv, G3).
 * Semantics match the static sad() in vpx_dsp/sad.c: sum of abs diffs.
 * Single vector accumulation across the whole block, one reduction at the end.
 * abs(a-b) on u8 wraps, so use max/min then subtract (d always in [0,255]).
 */
#include <riscv_vector.h>
#include <stdint.h>

#include "./sad_rvv.h"

static unsigned sad_rvv(const uint8_t *src, int src_stride,
                        const uint8_t *ref, int ref_stride,
                        int width, int height) {
  size_t vl = __riscv_vsetvlmax_e16m2();
  vuint16m2_t acc = __riscv_vmv_v_x_u16m2((uint16_t)0, vl);
  int y;

  if ((int)vl > width) vl = __riscv_vsetvl_e16m2((size_t)width);

  for (y = 0; y < height; y++) {
    int x = 0;
    while (x < width) {
      size_t v = (size_t)(width - x) < vl ? (size_t)(width - x) : vl;
      vuint8m1_t a = __riscv_vle8_v_u8m1(src + x, v);
      vuint8m1_t b = __riscv_vle8_v_u8m1(ref + x, v);
      vuint8m1_t hi = __riscv_vmaxu_vv_u8m1(a, b, v);
      vuint8m1_t lo = __riscv_vminu_vv_u8m1(a, b, v);
      vuint8m1_t d = __riscv_vsub_vv_u8m1(hi, lo, v);
      acc = __riscv_vwaddu_wv_u16m2(acc, d, v);
      x += (int)v;
    }
    src += src_stride;
    ref += ref_stride;
  }

  {
    vuint32m1_t s = __riscv_vwredsumu_vs_u16m2_u32m1(
        acc, __riscv_vmv_v_x_u32m1((uint32_t)0, vl), vl);
    return (unsigned)__riscv_vmv_x_s_u32m1_u32(s);
  }
}

/* Narrow blocks (w<=16): u16m1 accumulator, narrower chunk is faster. */
static unsigned sad_rvv_small(const uint8_t *src, int src_stride,
                              const uint8_t *ref, int ref_stride,
                              int width, int height) {
  size_t vmax = __riscv_vsetvlmax_e16m1();
  vuint16m1_t acc = __riscv_vmv_v_x_u16m1((uint16_t)0, vmax);
  int y;
  for (y = 0; y < height; y++) {
    int x = 0;
    while (x < width) {
      size_t v = (size_t)(width - x) < vmax ? (size_t)(width - x) : vmax;
      vuint8mf2_t a = __riscv_vle8_v_u8mf2(src + x, v);
      vuint8mf2_t b = __riscv_vle8_v_u8mf2(ref + x, v);
      vuint8mf2_t hi = __riscv_vmaxu_vv_u8mf2(a, b, v);
      vuint8mf2_t lo = __riscv_vminu_vv_u8mf2(a, b, v);
      acc = __riscv_vwaddu_wv_u16m1(acc, __riscv_vsub_vv_u8mf2(hi, lo, v), v);
      x += (int)v;
    }
    src += src_stride;
    ref += ref_stride;
  }
  {
    vuint32m1_t s = __riscv_vwredsumu_vs_u16m1_u32m1(
        acc, __riscv_vmv_v_x_u32m1((uint32_t)0, vmax), vmax);
    return (unsigned)__riscv_vmv_x_s_u32m1_u32(s);
  }
}

unsigned vpx_sad8x8_rvv(const uint8_t *src, int src_stride,
                        const uint8_t *ref, int ref_stride) {
  return sad_rvv_small(src, src_stride, ref, ref_stride, 8, 8);
}
unsigned vpx_sad16x16_rvv(const uint8_t *src, int src_stride,
                          const uint8_t *ref, int ref_stride) {
  return sad_rvv_small(src, src_stride, ref, ref_stride, 16, 16);
}
unsigned vpx_sad32x32_rvv(const uint8_t *src, int src_stride,
                          const uint8_t *ref, int ref_stride) {
  return sad_rvv(src, src_stride, ref, ref_stride, 32, 32);
}
unsigned vpx_sad64x64_rvv(const uint8_t *src, int src_stride,
                          const uint8_t *ref, int ref_stride) {
  return sad_rvv(src, src_stride, ref, ref_stride, 64, 64);
}
