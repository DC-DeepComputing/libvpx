---
name: libvpx/vpx_dsp/cast-64-bit-types-for-32-bit-simd-setters
description: When passing 64-bit integers or tran_high_t to 32-bit SIMD setter intrinsics like _mm_set_epi32 or _mm_setr_epi32, explicitly cast to (int) or use pair_set_epi32.
rubric: correctness
severity: should_fix
---

**Rule.** When passing 64-bit integer values or variable-width types (such as
tran_high_t in high bit-depth builds or 64-bit arithmetic expressions) to 32-bit
SIMD setter intrinsics like _mm_set_epi32(), _mm_setr_epi32(), or
_mm_set1_epi32(), explicitly cast the arguments to (int) or use helper macros
like pair_set_epi32() that include explicit casts.

**Why.** In high bit-depth builds, tran_high_t is typedef'd to int64_t. Passing
64-bit integer expressions to 32-bit SIMD setter intrinsics triggers MSVC
compiler conversion warnings (C4244) that Clang's -Wshorten-64-to-32 may not
catch, causing cross-platform build breakages under Visual Studio.

## Examples

Bad:

```cpp
// MSVC warning C4244: conversion from 'tran_high_t' (int64_t) to 'int'
const __m128i cospi_p16_p16 =
    _mm_setr_epi32(cospi_16_64 << 2, 0, cospi_16_64 << 2, 0);
const __m128i vec = _mm_set1_epi32(tran_coeff);
```

Good:

```cpp
// Explicitly cast to (int) or use helper macros like pair_set_epi32
const __m128i cospi_p16_p16 = _mm_setr_epi32((int)(cospi_16_64 << 2), 0,
                                             (int)(cospi_16_64 << 2), 0);
const __m128i pair_c = pair_set_epi32(c * 4, 0);
const __m128i vec = _mm_set1_epi32((int)tran_coeff);
```
