---
name: libvpx/vpx_dsp/keep-simd-lanes-consistent-across-build-configs
description: When writing conditional SIMD load or pack helpers across build configurations like CONFIG_VP9_HIGHBITDEPTH, keep unused register lanes consistent across branches by packing with zero.
rubric: readability
severity: should_fix
---

**Rule.** In conditional SIMD load and packing helpers branching on build
configurations (such as #if CONFIG_VP9_HIGHBITDEPTH versus standard #else),
ensure unused register lanes have identical contents across all branches. When
the standard branch zeroes upper lanes using _mm_loadl_epi64, pack high-bitdepth
elements against a zero vector (_mm_packs_epi32(in, zero)) rather than
duplicating elements (_mm_packs_epi32(in, in)).

**Why.** Inconsistent register lane contents across build configurations cause
downstream vector arithmetic and unpacking routines to diverge or read
unexpected duplicate data in one configuration. Zeroing upper lanes consistently
guarantees uniform register state regardless of whether high-bitdepth or
standard-bitdepth compilation is active.

## Examples

Bad:

```cpp
static INLINE __m128i load_input_data4(const tran_low_t *data) {
#if CONFIG_VP9_HIGHBITDEPTH
  const __m128i in = _mm_load_si128((const __m128i *)data);
  return _mm_packs_epi32(in, in);
#else
  return _mm_loadl_epi64((const __m128i *)data);
#endif
}
```

Good:

```cpp
static INLINE __m128i load_input_data4(const tran_low_t *data) {
#if CONFIG_VP9_HIGHBITDEPTH
  const __m128i zero = _mm_setzero_si128();
  const __m128i in = _mm_load_si128((const __m128i *)data);
  return _mm_packs_epi32(in, zero);
#else
  return _mm_loadl_epi64((const __m128i *)data);
#endif
}
```
