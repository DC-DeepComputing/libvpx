---
name: libvpx/chain-preprocessor-conditionals-with-elif
description: 'When structuring multi-branch preprocessor checks, chain mutually exclusive conditions with #elif or #elif defined(...) instead of nesting #if inside #else blocks'
rubric: readability
severity: should_fix
---

**Rule.** When checking mutually exclusive preprocessor conditions (such as
architecture, configuration, or platform features), chain alternative branches
using `#elif` (or `#elif defined(...)`) rather than nesting `#if`, `#ifdef`, or
`#ifndef` directives inside an `#else` block. Reserve nested preprocessor blocks
only for independent sub-conditions that apply strictly within a specific
branch.

**Why.** Nesting `#if` or `#ifdef` directly inside `#else` blocks introduces
unnecessary nesting depth, requires redundant `#endif` directives for each
nested branch, and obscures the priority order and fallback behavior across
alternative platforms and configurations. Flattening mutually exclusive
preprocessor checks into a single `#elif` ladder clarifies control flow and
makes all alternative configuration paths clear at a glance.

## Examples

Bad:

```cpp
// Nested #if/#ifdef inside #else for architecture checks:
#if HAVE_NEON
  uint8_t left[8];
#if VPX_WITH_ASAN
  vp8_zero_array(left, 8);
#endif
#else
#if HAVE_VSX
  uint8_t left[16];
#else
  uint8_t left[4];
#endif
#endif

// Nested #ifdef inside #else for platform timer selection:
#if defined(_WIN32)
  QueryPerformanceCounter(&t->begin);
#else
#ifdef CLOCK_MONOTONIC_RAW
  clock_gettime(CLOCK_MONOTONIC_RAW, &t->begin);
#else
  clock_gettime(CLOCK_MONOTONIC, &t->begin);
#endif
#endif
```

Good:

```cpp
// Flattened ladder with #elif:
#if HAVE_NEON
  uint8_t left[8];
#if VPX_WITH_ASAN
  vp8_zero_array(left, 8);
#endif
#elif HAVE_VSX
  uint8_t left[16];
#else
  uint8_t left[4];
#endif

// Flattened ladder with #elif defined(...):
#if defined(_WIN32)
  QueryPerformanceCounter(&t->begin);
#elif defined(CLOCK_MONOTONIC_RAW)
  clock_gettime(CLOCK_MONOTONIC_RAW, &t->begin);
#else
  clock_gettime(CLOCK_MONOTONIC, &t->begin);
#endif
```
