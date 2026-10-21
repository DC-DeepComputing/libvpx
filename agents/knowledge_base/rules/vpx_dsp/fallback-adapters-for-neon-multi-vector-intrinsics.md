---
name: libvpx/vpx_dsp/fallback-adapters-for-neon-multi-vector-intrinsics
rubric: correctness
severity: blocking
---

**Rule.** When introducing or using Arm Neon multi-vector load and store
intrinsics (such as vld1q_*_x2, vld1q_*_x3, or vld1q_*_x4), ensure fallback
adapters are provided in vpx_dsp/arm/mem_neon.h under compiler version checks
(e.g., #if defined(__GNUC__) && !defined(__clang__) checking __GNUC__ < 8 or
__GNUC__ < 9 and defined(__arm__)). Compose the multi-vector structures from
standard single-vector load/store operations so the code compiles on older GCC
versions and AArch32 toolchains.

**Why.** Multi-vector Neon intrinsics and struct types were introduced
incrementally across GCC versions (e.g., _x2 in GCC 8, _x3 in GCC 9) and are
often missing in AArch32 toolchains. Without fallback adapter implementations,
SIMD routines break libvpx continuous integration builders and legacy target
environments.

## Examples

Bad:

```cpp
// In vpx_dsp/arm/vpx_convolve8_neon_dotprod.c
// Using vld1q_u8_x3 directly without fallback definition causes compilation failure on GCC < 9 / AArch32.
const uint8x16x3_t permute_tbl = vld1q_u8_x3(dot_prod_permute_tbl);
```

Good:

```cpp
// In vpx_dsp/arm/mem_neon.h:
#if defined(__GNUC__) && !defined(__clang__)
#if __GNUC__ < 9 || defined(__arm__)
static INLINE uint8x16x3_t vld1q_u8_x3(uint8_t const *ptr) {
  uint8x16x3_t res = { { vld1q_u8(ptr + 0 * 16), vld1q_u8(ptr + 1 * 16),
                         vld1q_u8(ptr + 2 * 16) } };
  return res;
}
#endif
#endif
```
