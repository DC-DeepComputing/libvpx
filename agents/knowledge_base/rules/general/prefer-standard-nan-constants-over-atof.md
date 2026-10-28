---
name: libvpx/prefer-standard-nan-constants-over-atof
description: When generating or passing floating-point NaN values, use standard constants like NAN or std::nan instead of parsing string literals with atof
rubric: readability
severity: nit
---

**Rule.** Prefer standard floating-point constants and library functions—such as
NAN (from <math.h> or <cmath>), std::nan(), or
std::numeric_limits<T>::quiet_NaN()—over runtime string conversions like
atof("NaN") or strtod("NaN", NULL) when generating or initializing NaN values.

**Why.** Standard constants and functions provide direct, idiomatic
representations of NaN without the unnecessary runtime string-parsing overhead,
potential locale dependencies, or readability issues of atof("NaN").

## Examples

Bad:

```cpp
// Bad: Uses runtime string parsing to produce NaN values.
packets[1].count = atof("NaN");
double invalid_val = strtod("NaN", NULL);
```

Good:

```cpp
// Good: Uses standard floating-point constants or utilities.
packets[1].count = NAN;
double invalid_val = std::numeric_limits<double>::quiet_NaN();
```
