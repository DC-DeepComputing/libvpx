---
name: libvpx/document-non-obvious-simd-shuffle-masks
description: When using non-obvious SIMD shuffle masks, permutation patterns, or custom vector index arrangements, add comments explaining element ordering and algorithmic intent
rubric: readability
severity: nit
---

**Rule.** When writing SIMD routines that use non-obvious shuffle masks,
permutation constants, blend selectors, or custom element index vectors (such as
_mm_shuffle_epi32, _mm256_permutevar8x32_epi32, _MM_SHUFFLE, or vtbl), accompany
them with clarifying comments. Document the element mapping or rearranged order
and explain the algorithmic intent, such as matching reference C tie-breaking
order or reduction layout.

**Why.** Opaque numeric shuffle masks (such as 0xd8) and non-sequential index
vectors obscure how vector elements are reordered and why specific permutations
were chosen. Clarifying comments make vector manipulation understandable during
code review and prevent subtle tie-breaking or lane-ordering regressions during
maintenance.

## Examples

Bad:

```cpp
// Permute values and indices
v_loval_d = _mm_shuffle_epi32(v_sad_d, 0xd8);
v_loidx_d = _mm_set_epi32(3, 1, 2, 0);
```

Good:

```cpp
// Rearrange costs and indices so ties favor the lower candidate MV index
// to match the C reference implementation tie-breaking order.
v_loval_d = _mm_shuffle_epi32(v_sad_d, 0xd8);
v_loidx_d = _mm_set_epi32(3, 1, 2, 0);
```
