---
name: libvpx/use-standard-type-limits-not-hex-literals
description: When bounding, clamping, or comparing integers against type limits, use standard <limits.h> constants like UINT_MAX or INT_MAX instead of hardcoded hex literals like 0xffffffff
rubric: readability
severity: nit
---

**Rule.** When bounding, clamping, or comparing integer values against minimum
or maximum type boundaries (such as in VPXMIN, VPXMAX, clamp, or conditional
checks), use standard limit constants from <limits.h> or <stdint.h> (e.g.,
UINT_MAX, INT_MAX, INT64_MAX, UINT64_MAX, SIZE_MAX, INT_MIN). Do not use
hardcoded hexadecimal literals (such as 0xffffffff or 0x7fffffff) or magic
integer literals to represent type limits.

**Why.** Hardcoded hexadecimal literals (such as 0xffffffff) obscure intent,
assume specific type widths that may not be portable, and risk signedness
mismatch warnings or unexpected type promotions. Standard type limit constants
clearly communicate the intended boundary and portably match the target type's
width.

## Examples

Bad:

```cpp
// Hardcoded hex literals for type bounds
max_rate = (unsigned int)VPXMIN(0xffffffff, product);
if (val > 0x7fffffff) {
  val = 0x7fffffff;
}
```

Good:

```cpp
// Standard type limit constants from <limits.h> / <stdint.h>
max_rate = (unsigned int)VPXMIN(UINT_MAX, product);
if (val > INT_MAX) {
  val = INT_MAX;
}
```
