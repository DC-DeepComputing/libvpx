---
name: libvpx/vpx_dsp/use-tran-low-for-simd-coefficients
description: When implementing DSP transform coefficient functions, use tran_low_t and bitdepth conversion helpers instead of hardcoding int16_t or disabling high-bitdepth builds
rubric: correctness
severity: should_fix
---

**Rule.** In DSP transform and coefficient functions (such as Hadamard
transforms, DCT/IDCT, and quantization routines), use tran_low_t for coefficient
buffer pointers rather than hardcoding int16_t*. Use architecture-specific
bitdepth conversion helpers (such as load_tran_low and store_tran_low from
bitdepth_conversion_<arch>.h) so SIMD implementations remain available across
both standard and high-bitdepth (CONFIG_VP9_HIGHBITDEPTH) configurations, rather
than disabling SIMD routines with #if !CONFIG_VP9_HIGHBITDEPTH.

**Why.** In libvpx, transform coefficients are 16-bit (int16_t) in standard
configurations but expand to 32-bit (int32_t) when CONFIG_VP9_HIGHBITDEPTH is
enabled. Hardcoding int16_t* and guarding SIMD kernels with #if
!CONFIG_VP9_HIGHBITDEPTH disables SIMD optimization in high-bitdepth builds,
causing severe performance degradation. Using tran_low_t with architecture
bitdepth conversion helpers enables SIMD routines to execute seamlessly in both
builds.

## Examples

Bad:

```cpp
// Hardcoding int16_t* and guarding with !CONFIG_VP9_HIGHBITDEPTH disables SIMD in high-bitdepth builds.
#if !CONFIG_VP9_HIGHBITDEPTH
void vpx_hadamard_8x8_lsx(const int16_t *src, ptrdiff_t src_stride,
                          int16_t *dst) {
  // ... Hadamard butterfly computation ...
  __lsx_vst(res0, dst, 0);
  __lsx_vst(res1, dst, 16);
}
#endif  // !CONFIG_VP9_HIGHBITDEPTH
```

Good:

```cpp
// Using tran_low_t and store_tran_low supports both 16-bit and 32-bit coefficient builds.
#include "vpx_dsp/loongarch/bitdepth_conversion_lsx.h"

void vpx_hadamard_8x8_lsx(const int16_t *src, ptrdiff_t src_stride,
                          tran_low_t *dst) {
  // ... Hadamard butterfly computation ...
  store_tran_low(res0, dst, 0);
  store_tran_low(res1, dst, 8);
}
```
