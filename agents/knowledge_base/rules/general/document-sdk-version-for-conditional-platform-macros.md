---
name: libvpx/document-sdk-version-for-conditional-platform-macros
description: 'When guarding platform-specific feature detection or APIs with conditional #if defined(...) checks, document the minimum SDK or OS version requirement in an adjacent comment'
rubric: readability
severity: nit
---

**Rule.** When guarding platform-specific feature detection symbols, CPU flags,
or optional APIs behind conditional #if defined(...) checks, precede the check
with a comment documenting the minimum SDK, NDK, or OS release version that
introduced the symbol (e.g., Windows SDK version or macOS/Android API level). Do
not introduce conditional platform macro guards without documenting which SDK
version introduced them.

**Why.** Conditional #if defined(...) guards are often introduced to allow
compiling with older platform SDKs or toolchains while taking advantage of newer
runtime features when available. Documenting the specific SDK or OS release
directly in the code clarifies why the fallback guard exists, avoids losing
version context trapped in commit messages, and indicates when the check can
safely be retired once minimum toolchain requirements advance.

## Examples

Bad:

```cpp
#if HAVE_SVE
#if defined(PF_ARM_SVE_INSTRUCTIONS_AVAILABLE)
  if (IsProcessorFeaturePresent(PF_ARM_SVE_INSTRUCTIONS_AVAILABLE)) {
    flags |= HAS_SVE;
  }
#endif  // defined(PF_ARM_SVE_INSTRUCTIONS_AVAILABLE)
#endif  // HAVE_SVE
```

Good:

```cpp
#if HAVE_SVE
// Support for PF_ARM_SVE_INSTRUCTIONS_AVAILABLE was added in Windows SDK 26100.
#if defined(PF_ARM_SVE_INSTRUCTIONS_AVAILABLE)
  if (IsProcessorFeaturePresent(PF_ARM_SVE_INSTRUCTIONS_AVAILABLE)) {
    flags |= HAS_SVE;
  }
#endif  // defined(PF_ARM_SVE_INSTRUCTIONS_AVAILABLE)
#endif  // HAVE_SVE
```
