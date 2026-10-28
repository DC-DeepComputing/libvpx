---
name: libvpx/reuse-computed-simd-variables
description: When multiple variables require the result of the same SIMD intrinsic computation, reuse the computed variable directly instead of re-evaluating identical operations.
rubric: safety
severity: nit
---

**Rule.** When multiple variables require the result of the same SIMD intrinsic
calculation or expensive operation, assign subsequent variables directly from
the first computed variable instead of re-evaluating the identical operation. Do
not rely on compiler common subexpression elimination (CSE) for redundant
intrinsic invocations.

**Why.** Re-evaluating identical SIMD intrinsics obscures developer intent,
clutters code, and risks emitting duplicate instructions or wasting register
capacity if the compiler fails to optimize them away. Assigning already computed
vector variables explicitly guarantees direct register reuse.

## Examples

Bad:

```cpp
u0 = _mm_mulhrs_epi16(in[0], stk4_0);
u1 = _mm_mulhrs_epi16(in[0], stk4_0);
```

Good:

```cpp
u0 = _mm_mulhrs_epi16(in[0], stk4_0);
u1 = u0;
```
