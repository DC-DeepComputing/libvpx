---
name: libvpx/vpx_dsp/use-vqdmulhq-for-neon-fixed-point-multiplication
description: When computing 32-bit fixed-point multiplications like (a * b) >> 16 in ARM NEON, prescale multipliers and use vqdmulhq_s32 instead of 64-bit widening
rubric: safety
severity: nit
---

**Rule.** In ARM NEON SIMD kernels computing 32-bit fixed-point scaling or
multiplication of the form (a * b) >> 16 (such as in quantization routines),
prescale the multiplier operand by 15 bits (b << 15 via vshlq_n_s32) and use
vqdmulhq_s32 to perform the operation in 32-bit vectors. Avoid splitting 128-bit
quadwords into 64-bit doublewords to perform 64-bit widening multiplication
(vmull_s32), 64-bit right-shifting (vshrq_n_s64), and narrowing (vmovn_s64).

**Why.** vqdmulhq_s32(a, b << 15) computes saturate((a * (b << 15) * 2) >> 32),
which is mathematically equivalent to (a * b) >> 16 while keeping arithmetic
entirely within 4-lane 32-bit vector registers. Widening to 64-bit doubles
register usage, requires lane unpacking (vget_low_s32/vget_high_s32), and adds
significant instruction overhead from 64-bit shifts and narrowing.

## Examples

Bad:

```cpp
// Widening to 64-bit, shifting, and narrowing:
int64x2_t q0 = vmull_s32(vget_low_s32(coeff), vget_low_s32(quant));
int64x2_t q1 = vmull_s32(vget_high_s32(coeff), vget_high_s32(quant));
q0 = vshrq_n_s64(q0, 16);
q1 = vshrq_n_s64(q1, 16);
int32x4_t result = vcombine_s32(vmovn_s64(q0), vmovn_s64(q1));
```

Good:

```cpp
// Prescale multiplier by 15 and compute in 32-bit vectors directly:
int32x4_t quant_scaled = vshlq_n_s32(quant, 15);
int32x4_t result = vqdmulhq_s32(coeff, quant_scaled);
```
