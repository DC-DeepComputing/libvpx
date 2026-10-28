---
name: libvpx/use-setzero-to-initialize-zero-vectors
description: When initializing all-zero SIMD vector registers in x86 code, use dedicated zeroing intrinsics like _mm_setzero_*() instead of broadcasting zero with _mm_set1_*() or _mm_set_*()
rubric: readability
severity: should_fix
---

**Rule.** When initializing SIMD vector registers to all zeros, use dedicated
zeroing intrinsics (such as _mm_setzero_si128(), _mm256_setzero_si256(),
_mm512_setzero_si512(), or _mm_setzero_ps() / _mm256_setzero_ps()) instead of
broadcasting scalar zero with _mm*_set1_*() or passing duplicate zero arguments
to element-wise set functions like _mm*_set_*(). Apply this across all SSE,
AVX2, and AVX-512 integer and floating-point vector types (__m128i, __m256i,
__m512i, __m128, __m256, __m512).

**Why.** Dedicated _mm*_setzero_*() intrinsics are the canonical, idiomatic x86
intrinsics for producing all-zero vectors. They map directly to single
register-clearing instructions (such as pxor, vpxor, xorps, or vxorps) without
generating unnecessary constant-pool loads, redundant broadcast operations, or
element-type dependencies.

## Examples

Bad:

```cpp
// Bad: broadcasting scalar zero or passing duplicate zeros
const __m128i zero_sse = _mm_set1_epi32(0);
const __m256i zero_avx = _mm256_set1_epi16(0);
const __m128 zero_fp = _mm_set_ps(0.0f, 0.0f, 0.0f, 0.0f);
```

Good:

```cpp
// Good: canonical vector zeroing intrinsics
const __m128i zero_sse = _mm_setzero_si128();
const __m256i zero_avx = _mm256_setzero_si256();
const __m128 zero_fp = _mm_setzero_ps();
```
