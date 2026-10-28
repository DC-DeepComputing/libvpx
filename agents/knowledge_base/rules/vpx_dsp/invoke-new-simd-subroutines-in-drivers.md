---
name: libvpx/vpx_dsp/invoke-new-simd-subroutines-in-drivers
description: When implementing or declaring SIMD helper subroutines for high-bitdepth or alternate configurations, update outer driver functions to invoke the new routines
rubric: correctness
severity: should_fix
---

**Rule.** When implementing or declaring SIMD helper subroutines for
high-bitdepth or alternate configuration modes (such as `tran_low_t` coefficient
passes), ensure the corresponding outer driver functions and wrapper macros are
updated to invoke the new routines. Avoid leaving newly declared or defined
subroutines unreferenced in entry-point functions.

**Why.** Adding SIMD or assembly helper subroutines without updating caller
driver functions leaves the new optimized routines dead and results in
incomplete feature implementations that continue calling default or
lower-bitdepth routines.

## Examples

Bad:

```cpp
#if CONFIG_VP9_HIGHBITDEPTH
void vpx_idct16x16_256_add_neon_pass1_tran_low(const tran_low_t *input,
                                               int16_t *output);
#endif

void vpx_idct16x16_256_add_neon(const int16_t *input, uint8_t *dest,
                                int dest_stride) {
  // Still invokes the standard int16_t pass1 helper without using tran_low
  vpx_idct16x16_256_add_neon_pass1(input, pass1_output);
}
```

Good:

```cpp
#if CONFIG_VP9_HIGHBITDEPTH
void vpx_idct16x16_256_add_neon_pass1_tran_low(const tran_low_t *input,
                                               int16_t *output);
#else
#define vpx_idct16x16_256_add_neon_pass1_tran_low \
  vpx_idct16x16_256_add_neon_pass1
#endif

void vpx_idct16x16_256_add_neon(const tran_low_t *input, uint8_t *dest,
                                int dest_stride) {
  vpx_idct16x16_256_add_neon_pass1_tran_low(input, pass1_output);
}
```
