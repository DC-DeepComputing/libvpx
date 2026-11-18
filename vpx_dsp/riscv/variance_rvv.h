#ifndef VPX_DSP_RISCV_VARIANCE_RVV_H_
#define VPX_DSP_RISCV_VARIANCE_RVV_H_
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
int vpx_variance_rvv_applicable(int w, int h);
void vpx_variance_rvv(const uint8_t *src, int src_stride, const uint8_t *ref,
                      int ref_stride, int w, int h, uint32_t *sse, int *sum);
#ifdef __cplusplus
}
#endif
#endif
