---
name: libvpx/vpx_dsp/prefer-neon-rounding-shifts
rubric: safety
severity: should_fix
---

**Rule.** In ARM NEON SIMD implementations, use dedicated rounding right shift
intrinsics (such as vrshrq_n_s32, vrshrq_n_s16, or vrshrn_n_s32) for fixed-point
rounding shifts instead of manually duplicating rounding offsets with vdupq_n_*,
adding them with vaddq_*, and shifting with vshrq_n_*. Reserve manual addition
and bit-shifting for algorithms requiring asymmetric or non-standard rounding
offsets.

**Why.** ARM NEON provides single-instruction hardware rounding right shifts
that add (1 << (shift - 1)) and shift right in one operation. Synthesizing
rounding shifts manually requires generating vector constants and issuing
separate vector additions, increasing instruction count, register pressure, and
pipeline latency.

## Examples

Bad:

```cpp
static INLINE int32x4_t fdct_round_shift_s32(int32x4_t a) {
  const int32x4_t k_rounding = vdupq_n_s32(DCT_CONST_ROUNDING);
  return vshrq_n_s32(vaddq_s32(a, k_rounding), DCT_CONST_BITS);
}
```

Good:

```cpp
static INLINE int32x4_t fdct_round_shift_s32(int32x4_t a) {
  return vrshrq_n_s32(a, DCT_CONST_BITS);
}
```
