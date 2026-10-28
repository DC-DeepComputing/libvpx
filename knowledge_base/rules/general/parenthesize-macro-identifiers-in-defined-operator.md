---
name: libvpx/parenthesize-macro-identifiers-in-defined-operator
description: 'When using the preprocessor defined operator in #if or #elif directives, enclose all macro identifiers in parentheses like defined(MACRO)'
rubric: readability
severity: nit
---

**Rule.** Enclose macro identifiers in parentheses when using the `defined`
preprocessor operator in `#if` and `#elif` directives (e.g., `#if
defined(MACRO)` or `#elif !defined(MACRO)` instead of `#if defined MACRO`).
Maintain consistent parenthesized syntax across all preprocessor conditional
expressions.

**Why.** In libvpx C/C++ code, the `defined` preprocessor operator consistently
uses function-call-like parentheses around macro identifiers. Using
unparenthesized macro identifiers violates codebase style consistency and can
introduce ambiguity or readability issues in complex compound preprocessor
expressions.

## Examples

Bad:

```cpp
#if defined VPX_MAX_ALLOCABLE_MEMORY
  if (alloc_size > VPX_MAX_ALLOCABLE_MEMORY) return -1;
#endif

#if !defined CONFIG_VP9_ENCODER || defined CONFIG_MULTITHREAD
  return 0;
#endif
```

Good:

```cpp
#if defined(VPX_MAX_ALLOCABLE_MEMORY)
  if (alloc_size > VPX_MAX_ALLOCABLE_MEMORY) return -1;
#endif

#if !defined(CONFIG_VP9_ENCODER) || defined(CONFIG_MULTITHREAD)
  return 0;
#endif
```
