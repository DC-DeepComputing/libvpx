#ifndef VPX_DSP_RISCV_SAD_RVV_H_
#define VPX_DSP_RISCV_SAD_RVV_H_
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
unsigned vpx_sad8x8_rvv(const uint8_t *src, int src_stride, const uint8_t *ref, int ref_stride);
unsigned vpx_sad16x16_rvv(const uint8_t *src, int src_stride, const uint8_t *ref, int ref_stride);
unsigned vpx_sad32x32_rvv(const uint8_t *src, int src_stride, const uint8_t *ref, int ref_stride);
unsigned vpx_sad64x64_rvv(const uint8_t *src, int src_stride, const uint8_t *ref, int ref_stride);
#ifdef __cplusplus
}
#endif
#endif
