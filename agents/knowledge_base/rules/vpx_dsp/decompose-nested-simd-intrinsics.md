---
name: libvpx/vpx_dsp/decompose-nested-simd-intrinsics
description: When implementing SIMD routines, decompose multi-step intrinsics and nested store arguments into named intermediate stage variables rather than inlining expressions
rubric: readability
severity: nit
---

**Rule.** In SIMD and DSP implementations, decompose deeply nested intrinsic
expressions and multi-step vector operations into separate statements using
named intermediate stage variables (e.g., `a0..a3`, `b0..b3`, `c0..c3`, or
descriptive names; qualified with `const` where immutable). Avoid nesting
intrinsic calculations directly inside store helper arguments, macro
invocations, or return statements, and keep pointer offset arithmetic clean
without redundant grouping parentheses.

**Why.** Inlining complex intrinsic operations inside store helpers or return
statements obscures dataflow, complicates debugging, and breaks structural
consistency across transform size variants. Assigning intermediate results to
named stage variables clarifies each computation stage and produces cleaner
pointer offset expressions without adding runtime overhead.

## Examples

Bad:

```cpp
// Deeply nested intrinsics inline in helper return
static INLINE int16x8_t sub_round_shift_s16(const int16x8_t a) {
  return vrshrq_n_s16(vsubq_s16(a, vreinterpretq_s16_u16(vshrq_n_u16(
                                       vreinterpretq_u16_s16(a), 15))),
                      2);
}

// Inlining intrinsic computations and redundant parens in store helper
store_s32q_to_tran_low((coeff + 4 * i), vhaddq_s32(b0, b2));
store_s32q_to_tran_low((coeff + 4 * i) + 256, vhaddq_s32(b1, b3));
```

Good:

```cpp
// Decomposed into named intermediate variables
static INLINE int16x8_t sub_round_shift_s16(const int16x8_t a) {
  const uint16x8_t a_u16 = vreinterpretq_u16_s16(a);
  const uint16x8_t a_sign_u16 = vshrq_n_u16(a_u16, 15);
  const int16x8_t a_sign_s16 = vreinterpretq_s16_u16(a_sign_u16);
  return vrshrq_n_s16(vsubq_s16(a, a_sign_s16), 2);
}

// Staged intermediate variables and clean pointer offset arithmetic
const int32x4_t c0 = vhaddq_s32(b0, b2);
const int32x4_t c1 = vhaddq_s32(b1, b3);
store_s32q_to_tran_low(coeff + 4 * i, c0);
store_s32q_to_tran_low(coeff + 4 * i + 256, c1);
```
