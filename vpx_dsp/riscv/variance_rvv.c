/* RVV variance kernel (ported from open-risc-v/libvpx-rvv, G3).
 * Outputs both sum(diff) and sum(diff^2) as exact integers: the caller's
 * `sse - sum*sum/(w*h)` is rounding-sensitive, so no float approximation.
 * Two i32m4 accumulators across rows, reduced once per 30000 pixels.
 * Bit-exact vs the C variance() (pure integer, no tolerance).
 */
#include <stdint.h>
#include <riscv_vector.h>

#include "vpx_dsp/riscv/variance_rvv.h"

#if defined(__riscv_vector)

#define VAR_FLUSH_PIXELS 30000

int vpx_variance_rvv_applicable(int w, int h) { return w >= 16 && h >= 4; }

void vpx_variance_rvv(const uint8_t *src, int src_stride, const uint8_t *ref,
                      int ref_stride, int w, int h, uint32_t *sse, int *sum) {
  int64_t tss = 0;
  int64_t tsum = 0;
  int rows_per_flush;
  int rows_since = 0;
  size_t vlmax;
  vint32m4_t acc_ss, acc_s;
  int32_t tmp_ss[64], tmp_s[64];
  int y, x, i;

  vlmax = __riscv_vsetvl_e8m1((size_t)w);
  acc_ss = __riscv_vmv_v_x_i32m4(0, vlmax);
  acc_s = __riscv_vmv_v_x_i32m4(0, vlmax);

  rows_per_flush = VAR_FLUSH_PIXELS / w;
  if (rows_per_flush < 1) rows_per_flush = 1;

  for (y = 0; y < h; ++y) {
    for (x = 0; x < w; x += (int)vlmax) {
      size_t vl = __riscv_vsetvl_e8m1((size_t)(w - x));
      vuint8m1_t va = __riscv_vle8_v_u8m1(src + x, vl);
      vuint8m1_t vb = __riscv_vle8_v_u8m1(ref + x, vl);
      vint16m2_t da =
          __riscv_vreinterpret_v_u16m2_i16m2(__riscv_vzext_vf2_u16m2(va, vl));
      vint16m2_t db =
          __riscv_vreinterpret_v_u16m2_i16m2(__riscv_vzext_vf2_u16m2(vb, vl));
      vint16m2_t d = __riscv_vsub_vv_i16m2(da, db, vl);

      acc_s = __riscv_vwadd_wv_i32m4(acc_s, d, vl);
      acc_ss = __riscv_vwmacc_vv_i32m4(acc_ss, d, d, vl);
    }

    src += src_stride;
    ref += ref_stride;
    ++rows_since;

    if (rows_since == rows_per_flush || y == h - 1) {
      __riscv_vse32_v_i32m4(tmp_ss, acc_ss, vlmax);
      __riscv_vse32_v_i32m4(tmp_s, acc_s, vlmax);
      for (i = 0; i < (int)vlmax; ++i) {
        tss += tmp_ss[i];
        tsum += tmp_s[i];
      }
      acc_ss = __riscv_vmv_v_x_i32m4(0, vlmax);
      acc_s = __riscv_vmv_v_x_i32m4(0, vlmax);
      rows_since = 0;
    }
  }

  *sse = (uint32_t)tss;
  *sum = (int)tsum;
}

#endif
