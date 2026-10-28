---
name: libvpx/verify-legacy-compiler-fallback-intrinsics
description: When writing compiler compatibility fallbacks for legacy toolchains (such as MSVC < 1910), ensure substitute intrinsics are supported; for missing AVX2 extractions, use `_mm256_extracti128_si256` and SSE intrinsics.
rubric: correctness
severity: should_fix
---

**Rule.** When implementing compiler-compatibility fallback paths (such as `#if
defined(_MSC_VER) && (_MSC_VER < 1910)` for legacy toolchains), ensure that all
substitute intrinsics are fully supported in the targeted compiler version.
Avoid replacing an unsupported intrinsic with another intrinsic that is also
missing from that compiler version (for example, replacing
`_mm256_extract_epi16` with `_mm256_cvtsi256_si32`, which was also unsupported
in MSVC prior to VS 2017). Instead, use base intrinsics available in that
toolchain, such as extracting lower vector lanes via `_mm256_extracti128_si256`
or `_mm256_castsi256_si128` and operating with standard SSE intrinsics such as
`_mm_cvtsi128_si32(...) & 0xffff`.

**Why.** Older compiler releases (such as Visual Studio 2015 / MSVC 19.00 /
`_MSC_VER < 1910`) lack support for several AVX2 intrinsics like
`_mm256_extract_epi16` and `_mm256_cvtsi256_si32`. Introducing fallback paths
that rely on equally unsupported intrinsics fails to resolve compilation breaks
on legacy build configurations. Extracting the lower 128-bit lane with
`_mm256_extracti128_si256` and operating with baseline SSE2 intrinsics like
`_mm_cvtsi128_si32` provides a portable, zero-overhead workaround that compiles
cleanly across legacy MSVC toolchains.

## Examples

Bad:

```cpp
#if defined(_MSC_VER) && (_MSC_VER < 1910)
  // _mm256_cvtsi256_si32 is also unsupported in MSVC 2015 (_MSC_VER < 1910)
  return (_mm256_cvtsi256_si32(eob) & 0xffff);
#else
  return (uint16_t)_mm256_extract_epi16(eob, 0);
#endif
```

Good:

```cpp
#if defined(_MSC_VER) && (_MSC_VER < 1910)
  // Extract lower 128-bit lane and use standard SSE2 intrinsic
  return _mm_cvtsi128_si32(_mm256_extracti128_si256(eob, 0)) & 0xffff;
#else
  return (uint16_t)_mm256_extract_epi16(eob, 0);
#endif
```
