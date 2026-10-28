---
name: libvpx/vpx_dsp/distinguish-multiply-add-from-multiply-accumulate
description: When naming SIMD and DSP helper functions that multiply and sum products without a preexisting accumulator, use multiply_add rather than multiply_accumulate
rubric: readability
severity: should_fix
---

**Rule.** When naming SIMD and DSP arithmetic helper functions, use
`multiply_add` (or `madd`) rather than `multiply_accumulate` (or `mac` / `mla`)
when the operation computes products and sums them together without adding into
a preexisting accumulator register. Reserve `multiply_accumulate` strictly for
routines that take an existing accumulator vector and add product terms into it
(e.g., `acc + a * b`).

**Why.** Multiply-accumulate (MAC) conventionally implies accumulating terms
into a running total or preexisting destination register. Using
`multiply_accumulate` for standalone pairwise multiplication and addition
creates misleading function names that confuse readers about whether an initial
accumulator parameter is expected or modified.

## Examples

Bad:

```cpp
// Anti-pattern: Named multiply_accumulate despite computing standalone products
static INLINE __m128i multiply_accumulate_epi8_sse2(
    const __m128i *const src_1, const __m128i *const src_2,
    const __m128i *const ker_1, const __m128i *const ker_2) {
  const __m128i src_1_half = _mm_unpacklo_epi8(*src_1, _mm_setzero_si128());
  const __m128i src_2_half = _mm_unpacklo_epi8(*src_2, _mm_setzero_si128());
  const __m128i madd_1 = _mm_madd_epi16(src_1_half, *ker_1);
  const __m128i madd_2 = _mm_madd_epi16(src_2_half, *ker_2);
  return _mm_add_epi32(madd_1, madd_2);
}
```

Good:

```cpp
// Correct: Named multiply_add to reflect summing products without an accumulator
static INLINE __m128i multiply_add_epi8_sse2(
    const __m128i *const src_1, const __m128i *const src_2,
    const __m128i *const ker_1, const __m128i *const ker_2) {
  const __m128i src_1_half = _mm_unpacklo_epi8(*src_1, _mm_setzero_si128());
  const __m128i src_2_half = _mm_unpacklo_epi8(*src_2, _mm_setzero_si128());
  const __m128i madd_1 = _mm_madd_epi16(src_1_half, *ker_1);
  const __m128i madd_2 = _mm_madd_epi16(src_2_half, *ker_2);
  return _mm_add_epi32(madd_1, madd_2);
}
```
