---
name: libvpx/use-standard-interleave-terminology-in-simd-comments
description: When documenting SIMD operations or data layouts that alternate elements across vectors, use standard terminology like interleave rather than non-standard terms like interweave
rubric: readability
severity: nit
---

**Rule.** When documenting SIMD operations, memory transformations, or data
layouts that combine alternating elements from multiple vectors or buffers, use
canonical technical terms such as 'interleave' and 'deinterleave' rather than
non-standard synonyms like 'interweave'. Consistent terminology ensures code
comments match hardware instruction semantics and codebase conventions.

**Why.** 'Interleave' is the standard industry and codebase terminology across
SIMD architectures (such as x86 unpack and ARM zip/uzp intrinsics) and planar
video formats. Using standard terminology maintains clarity, avoids ambiguity,
and ensures comments are searchable across the project.

## Examples

Bad:

```cpp
// Interweaves even and odd pixels from src1 and src2 into 16-bit words.
const __m128i lo = _mm_unpacklo_epi8(src1, src2);
```

Good:

```cpp
// Interleaves even and odd pixels from src1 and src2 into 16-bit words.
const __m128i lo = _mm_unpacklo_epi8(src1, src2);
```
