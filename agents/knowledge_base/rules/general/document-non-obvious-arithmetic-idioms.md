---
name: libvpx/document-non-obvious-arithmetic-idioms
description: When implementing SIMD or DSP routines with non-obvious arithmetic, rounding, branch-free masks/biases, or algebraic shortcuts, document the equivalent scalar formula and mechanism.
rubric: readability
severity: nit
---

**Rule.** When implementing SIMD or DSP routines that use non-obvious
arithmetic, signed rounding/division idioms, branch-free comparison masks and
bias vectors, or specialized algebraic simplifications (such as shift
compensations or instruction substitutions like `vec_madds` for `vec_mulhi`),
accompany the code with an explanatory comment detailing the underlying scalar
algebraic formula or branch-free mechanism.

**Why.** Low-level vector intrinsics, branch-free bias masks (e.g., biasing
elements to avoid zero checks or branchless increments), signed rounding
adjustments (e.g., sign extraction before right shifts), and block-size
algebraic shortcuts are opaque and can look like accidental bugs or deviations
from the codec specification. Stating the equivalent scalar mathematical formula
and derivation makes the code maintainable, verifiable during code review, and
portable across SIMD architectures.

## Examples

Bad:

```cpp
// Variant 1: SIMD signed rounding arithmetic
static INLINE void right_shift_8x8(int16x8_t *res, const int bit) {
  int16x8_t sign0 = vshrq_n_s16(res[0], 15);
  res[0] = vshrq_n_s16(vaddq_s16(vaddq_s16(res[0], vdupq_n_s16(1)), sign0), bit);
}

// Variant 2: Branch-free mask and bias adjustments
mask = vreinterpret_s16_u16(vceq_s16(input_0, nonzero_bias_a));
input_0 = vadd_s16(vadd_s16(input_0, mask), nonzero_bias_b);

// Variant 3: DSP algebraic shortcut / compensation
static INLINE int16x8_t quantize_coeff_32(int16x8_t coeff, int16x8_t round,
                                          int16x8_t quant_shift, bool16x8_t mask) {
  int16x8_t qcoeff = vec_madds(vec_vaddshs(coeff, round), quant_shift, vec_zeros_s16);
  return vec_and(qcoeff, mask);
}
```

Good:

```cpp
// Variant 1: Document equivalent scalar formula for signed rounding
/* Computes: out[j] = (temp[j] + 1 + (temp[j] < 0)) >> bit; */
static INLINE void right_shift_8x8(int16x8_t *res, const int bit) {
  int16x8_t sign0 = vshrq_n_s16(res[0], 15);
  res[0] = vshrq_n_s16(vaddq_s16(vaddq_s16(res[0], vdupq_n_s16(1)), sign0), bit);
}

// Variant 2: Document branch-free mask purpose to avoid an 'if' branch
// Increase non-zero first elements by 1: compare with bias_a, adjust with bias_b.
mask = vreinterpret_s16_u16(vceq_s16(input_0, nonzero_bias_a));
input_0 = vadd_s16(vadd_s16(input_0, mask), nonzero_bias_b);

// Variant 3: Document algebraic shortcut and shift compensation
// 32x32 blocks require 2x multiplication, compensating for vec_mulhi shift:
// (((a * b) >> 15) >> 1) << 1 == (a * b >> 15), so vec_madds can be used directly.
static INLINE int16x8_t quantize_coeff_32(int16x8_t coeff, int16x8_t round,
                                          int16x8_t quant_shift, bool16x8_t mask) {
  int16x8_t qcoeff = vec_madds(vec_vaddshs(coeff, round), quant_shift, vec_zeros_s16);
  return vec_and(qcoeff, mask);
}
```
