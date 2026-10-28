---
name: libvpx/cast-double-to-float-in-math-classification-macros
description: When calling floating-point classification macros like isfinite or isnan on double values in C, cast arguments to float to prevent MinGW -Wfloat-conversion warnings
rubric: correctness
severity: nit
---

**Rule.** When invoking floating-point classification macros (such as
isfinite(), isnan(), isinf(), or isnormal()) on double values in C source files,
explicitly cast the argument to float (e.g., isfinite((float)val)). On certain
MinGW / Windows GCC toolchains (such as x86_64-win64-gcc), macro expansions
invoke float helpers that trigger -Wfloat-conversion warnings when passed uncast
double expressions.

**Why.** In libvpx, -Wfloat-conversion is enabled globally via configure.
Certain MinGW <math.h> implementations expand floating-point classification
macros via float-based internal helper functions, causing -Wfloat-conversion
compiler warnings (and build failures under -Werror) unless double arguments are
explicitly converted to float.

## Examples

Bad:

```cpp
if (!isfinite(stats->count) || stats->count < 0.0) {
  ERROR("Invalid stats");
}
```

Good:

```cpp
if (!isfinite((float)stats->count) || stats->count < 0.0) {
  ERROR("Invalid stats");
}
```
