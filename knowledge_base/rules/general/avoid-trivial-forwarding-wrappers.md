---
name: libvpx/avoid-trivial-forwarding-wrappers
description: When writing helper functions, avoid trivial forwarding wrappers or numbered aliases that merely reorder arguments or pass constants; generalize parameters and invoke helpers directly.
rubric: readability
severity: should_fix
---

**Rule.** Avoid introducing trivial forwarding wrapper functions or numbered
helper aliases (e.g., func2) that merely pass arguments unchanged, forward
hardcoded constants, or rename/reorder parameters for symmetric or commutative
operations. Instead, use generalized parameter names with clarifying doc
comments in a single helper and invoke it directly with explicit arguments at
call sites.

**Why.** Thin forwarding wrappers and numbered function aliases add unnecessary
indirection, declaration boilerplate, and cognitive load without meaningful
semantic abstraction. Generalizing parameter names in a single helper and
calling it directly keeps data flow explicit and eliminates redundant wrapper
code.

## Examples

Bad:

```cpp
// Bad: Trivial 1-to-1 forwarding wrapper with unchanged arguments
static INLINE void filter_8x2_2t_pixels(const __m256i *sig, const __m256i *f,
                                        __m256i *y0, __m256i *y1) {
  filter_16_2t_pixels(sig, f, y0, y1);
}

// Bad: Numbered alias wrapper created solely for parameter reordering
static INLINE __m128i idct_calc_wraplow2_sse2(const __m128i cospi0,
                                              const __m128i cospi1,
                                              const __m128i in) {
  return idct_calc_wraplow_sse2(cospi0, cospi1, in);
}
```

Good:

```cpp
// Good: Invoke the underlying helper directly at call sites
filter_16_2t_pixels(signal, &ff, &res0, &res1);

// Good: Generalize parameter names in a single helper and call directly
// Calculate the dot product between in0/1 and x and wrap to short.
static INLINE __m128i idct_calc_wraplow_sse2(const __m128i in0,
                                             const __m128i in1,
                                             const __m128i x) {
  const __m128i t0 = idct_madd_round_shift_sse2(in0, x);
  const __m128i t1 = idct_madd_round_shift_sse2(in1, x);
  return _mm_packs_epi32(t0, t1);
}
```
