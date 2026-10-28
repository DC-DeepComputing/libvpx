---
name: libvpx/vpx_dsp/use-cvtsi32-for-low-lane-scalar-initialization
description: When initializing a SIMD vector where only the lowest lane is needed for computation, use _mm_cvtsi32_si128 instead of multi-lane broadcast functions
rubric: safety
severity: should_fix
---

**Rule.** When initializing a 128-bit vector constant or scalar operand where
only the lowest lane is needed for calculation (such as scalar rounding offsets
or DC sums prior to broadcasting), use _mm_cvtsi32_si128() instead of populating
all lanes with _mm_set1_*() or _mm_set_*(). This emits a single 32-bit scalar
move instruction rather than multi-instruction broadcast sequences or 128-bit
constant pool memory loads.

**Why.** _mm_cvtsi32_si128() compiles to a fast scalar-to-vector move (movd),
avoiding unnecessary vector constant table entries, memory loads, and multi-lane
broadcast instructions when intermediate calculations only operate on the lowest
element.

## Examples

Bad:

```cpp
// Unnecessary broadcast or multi-lane constant table load when only lowest lane is used
const __m128i two = _mm_set_epi16(2, 2, 2, 2, 2, 2, 2, 2);
const __m128i dc = _mm_srli_epi16(_mm_add_epi16(sum, two), 2);
```

Good:

```cpp
// Single movd instruction into lowest lane
const __m128i two = _mm_cvtsi32_si128(2);
const __m128i dc = _mm_srli_epi16(_mm_add_epi16(sum, two), 2);
```
