---
name: libvpx/vpx_dsp/avoid-trivial-simd-wrapper-functions
description: When writing SIMD or DSP routines, avoid defining trivial helper functions that merely wrap a single intrinsic; call the standard SIMD instruction directly
rubric: design
severity: should_fix
---

**Rule.** Do not define trivial helper functions or wrapper subroutines that
merely forward to a single SIMD intrinsic or instruction (such as NEON
vrshrq_n_*, vaddq_*, or x86 _mm_*). Call the standard compiler intrinsic
directly at call sites unless the helper combines multiple operations,
encapsulates chained multi-step conversions, or provides a unified
cross-platform abstraction.

**Why.** Trivial single-instruction wrappers add unnecessary boilerplate and
layer indirection over standard compiler intrinsics without providing type
safety or semantic clarity, making the underlying instruction stream harder to
inspect.

## Examples

Bad:

```cpp
static INLINE int32x4_t fdct_round_shift_s32(const int32x4_t a) {
  return vrshrq_n_s32(a, DCT_CONST_BITS);
}

// Call site:
out[0] = fdct_round_shift_s32(u[0]);
```

Good:

```cpp
// Call the standard SIMD intrinsic directly at call sites
out[0] = vrshrq_n_s32(u[0], DCT_CONST_BITS);
```
