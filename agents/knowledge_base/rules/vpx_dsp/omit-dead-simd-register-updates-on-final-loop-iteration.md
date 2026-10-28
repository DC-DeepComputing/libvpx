---
name: libvpx/vpx_dsp/omit-dead-simd-register-updates-on-final-loop-iteration
description: In unrolled SIMD loops with trailing inter-iteration vector register updates, terminate the loop early and place final stores outside to avoid dead vector operations
rubric: safety
severity: should_fix
---

**Rule.** In unrolled SIMD kernel loops where trailing vector operations (such
as shifts, byte alignments, or blends) update registers to prepare data for
subsequent iterations, peel or break out the final iteration. Terminate the loop
one iteration early and execute only the required memory stores outside the loop
to avoid computing dead vector updates on the final pass.

**Why.** When inter-iteration vector transformations (e.g. _mm_alignr_epi8,
_mm_slli_si128, or vextq_u8) execute at the end of an unrolled loop body, the
final pass computes vector values that are discarded when the loop exits.
Peeling the final iteration's stores eliminates wasted SIMD instructions and
reduces register pressure in performance-critical kernels.

## Examples

Bad:

```cpp
for (i = 0; i < 32; i += 2) {
  _mm_store_si128((__m128i *)dst, avg2_0);
  dst += stride;
  _mm_store_si128((__m128i *)dst, avg3_0);
  dst += stride;
  avg2_0 = _mm_alignr_epi8(avg2_1, avg2_0, 2);
  avg2_1 = _mm_alignr_epi8(AR, avg2_1, 2);
  avg3_0 = _mm_alignr_epi8(avg3_1, avg3_0, 2);
  avg3_1 = _mm_alignr_epi8(AR, avg3_1, 2);
}
```

Good:

```cpp
for (i = 0; i < 30; i += 2) {
  _mm_store_si128((__m128i *)dst, avg2_0);
  dst += stride;
  _mm_store_si128((__m128i *)dst, avg3_0);
  dst += stride;
  avg2_0 = _mm_alignr_epi8(avg2_1, avg2_0, 2);
  avg2_1 = _mm_alignr_epi8(AR, avg2_1, 2);
  avg3_0 = _mm_alignr_epi8(avg3_1, avg3_0, 2);
  avg3_1 = _mm_alignr_epi8(AR, avg3_1, 2);
}
_mm_store_si128((__m128i *)dst, avg2_0);
dst += stride;
_mm_store_si128((__m128i *)dst, avg3_0);
```
