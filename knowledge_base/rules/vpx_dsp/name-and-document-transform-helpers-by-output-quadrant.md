---
name: libvpx/vpx_dsp/name-and-document-transform-helpers-by-output-quadrant
description: When splitting large transform implementations into stage helpers, name helpers by their output quadrant and document input indices and output ranges
rubric: readability
severity: nit
---

**Rule.** When splitting large transform implementations (such as 32x32
IDCT/DCT) into decomposed stage helper functions, name the helper functions
after the specific output quadrant or range they compute (e.g., _quarter_1,
_quarter_2, _quarter_3_4) rather than vague partition names (e.g.,
_first_half_1, _first_half_2). Precede each helper function with a comment
documenting the input indices and output pixel ranges.

**Why.** Vague subdivision names like first_half_1 and first_half_2 obscure
which mathematical subset of the transform is being processed. Standard quadrant
naming (_quarter_1 for outputs 0–7, _quarter_2 for outputs 8–15, _quarter_3_4
for outputs 16–31) paired with input/output comments clarifies dataflow and
maintains consistency across SIMD and assembly transform implementations.

## Examples

Bad:

```cpp
// Stage (A): 0, 1, 2, 3, 4, 5, 6, 7
static void idct32_8x16_135_first_half_1(const __m128i *in, __m128i *out);

// Stage (A): 8, 9, 10, 11, 12, 13, 14, 15
static void idct32_8x16_135_first_half_2(const __m128i *in, __m128i *out);
```

Good:

```cpp
// For each 8x32 block __m128i in[32],
// Input with index, 0, 4, 8, 12
// output pixels: 0-7 in __m128i out[32]
static INLINE void idct32_135_8x32_quarter_1(const __m128i *const in,
                                             __m128i *const out);

// For each 8x32 block __m128i in[32],
// Input with index, 2, 6, 10, 14
// output pixels: 8-15 in __m128i out[32]
static INLINE void idct32_135_8x32_quarter_2(const __m128i *const in,
                                             __m128i *const out);
```
