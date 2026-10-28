---
name: libvpx/use-if-for-have-and-config-macros
description: 'When conditionally compiling feature or architecture code using CONFIG_* or HAVE_* macros, use #if or #elif instead of #ifdef, #ifndef, or defined()'
rubric: correctness
severity: should_fix
---

**Rule.** Use `#if CONFIG_*` and `#if HAVE_*` (or `#elif`, `#if !CONFIG_*`, `#if
!HAVE_*`) rather than `#ifdef`, `#ifndef`, or `#if defined(...)` when
conditionally compiling feature- or architecture-specific code.

**Why.** In libvpx, configuration and capability macros (such as `CONFIG_*`,
`HAVE_NEON`, `HAVE_SSE2`, `HAVE_VSX`) are generated in `vpx_config.h` and
unconditionally defined as either `1` (enabled) or `0` (disabled). Because
`#ifdef`, `#ifndef`, and `defined()` test macro existence rather than numeric
value, `#ifdef HAVE_FEATURE` evaluates to true even when the feature is
explicitly disabled (`#define HAVE_FEATURE 0`), leading to compiling incorrect
architecture paths or build breakages.

## Examples

Bad:

```cpp
// Incorrect: HAVE_VSX and CONFIG_VP9_HIGHBITDEPTH are defined to 0 when disabled, so #ifdef evaluates to true.
#ifdef HAVE_VSX
  unsigned char uleft_col[16];
#else
  unsigned char uleft_col[8];
#endif

#ifdef CONFIG_VP9_HIGHBITDEPTH
  vpx_highbd_clear_system_state();
#endif
```

Good:

```cpp
// Correct: Evaluates the numeric definition (0 or 1).
#if HAVE_VSX
  unsigned char uleft_col[16];
#else
  unsigned char uleft_col[8];
#endif

#if CONFIG_VP9_HIGHBITDEPTH
  vpx_highbd_clear_system_state();
#endif
```
