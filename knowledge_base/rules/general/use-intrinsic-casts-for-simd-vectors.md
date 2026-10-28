---
name: libvpx/use-intrinsic-casts-for-simd-vectors
description: When reinterpreting between x86 SIMD vector types (__m128, __m128i, __m128d), use intrinsic cast functions like _mm_castsi128_ps() instead of C-style casts
rubric: correctness
severity: should_fix
---

**Rule.** When reinterpreting between x86 SIMD vector types (such as __m128,
__m128i, and __m128d or their 256/512-bit variants), use standard intrinsic cast
functions (such as _mm_castsi128_ps(), _mm_castps_si128(), _mm_castsi128_pd())
instead of C-style type casts like (__m128) or (__m128i).

**Why.** C-style casts between SIMD vector types rely on compiler-specific
vector extensions (such as GCC vector extensions) and fail to compile on
toolchains like MSVC where SIMD types are distinct struct or union definitions.
Standard _mm_cast* intrinsics provide portable, zero-overhead type
reinterpretation across all supported compilers.

## Examples

Bad:

```cpp
// C-style vector cast relies on compiler extensions and fails on MSVC.
const __m128i r_0_2 = (__m128i)_mm_loadh_pi(
    (__m128)r_0, (const __m64 *)(ref + 2 * ref_stride));
```

Good:

```cpp
// Intrinsic cast functions provide portable zero-overhead reinterpretation.
const __m128i r_0_2 = _mm_castps_si128(
    _mm_loadh_pi(_mm_castsi128_ps(r_0), (const __m64 *)(ref + 2 * ref_stride)));
```
