---
name: libvpx/prefer-mm-set1-for-scalar-broadcast
description: When broadcasting a scalar value across all SIMD vector lanes in x86 code, use _mm_set1_* intrinsics instead of repeated duplicate arguments to _mm_set_*
rubric: readability
severity: should_fix
---

**Rule.** When initializing a SIMD vector with the same scalar value in every
lane, use broadcast intrinsics (such as _mm_set1_epi16(), _mm_set1_epi32(),
_mm_set1_epi8(), or their AVX/float counterparts) instead of passing identical
duplicate arguments to element-wise set intrinsics like _mm_set_*() or
_mm_setr_*().

**Why.** Using _mm_set1_*() expresses the broadcast intent directly, improves
readability by eliminating redundant argument lists, and avoids subtle errors
from mismatched duplicate arguments across vector lanes.

## Examples

Bad:

```cpp
const __m128i two = _mm_set_epi16(2, 2, 2, 2, 2, 2, 2, 2);
const __m128i mask = _mm_set_epi32(val, val, val, val);
```

Good:

```cpp
const __m128i two = _mm_set1_epi16(2);
const __m128i mask = _mm_set1_epi32(val);
```
