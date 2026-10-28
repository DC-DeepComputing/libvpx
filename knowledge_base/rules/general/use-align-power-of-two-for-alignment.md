---
name: libvpx/use-align-power-of-two-for-alignment
description: When aligning buffer dimensions, strides, or offsets to a power of two, use ALIGN_POWER_OF_TWO instead of manual bitwise expressions
rubric: readability
severity: nit
---

**Rule.** When aligning dimensions, strides, buffer sizes, or offsets to a power
of two (2^n), use the ALIGN_POWER_OF_TWO(value, n) helper macro from
vpx_ports/mem.h. Do not open-code manual bitwise alignment arithmetic such as (x
+ (1 << n) - 1) & ~((1 << n) - 1) or (x + mask) & ~mask.

**Why.** Using ALIGN_POWER_OF_TWO clarifies alignment intent, prevents operator
precedence or bitmask arithmetic bugs, and ensures consistent alignment idioms
across codec routines.

## Examples

Bad:

```cpp
// Manual bitmask alignment arithmetic
int aligned_width = (width + 7) & ~7;
buf->img.y_stride = ((aligned_width + 2 * buf->img.border) + 31) & ~31;
```

Good:

```cpp
// Use ALIGN_POWER_OF_TWO(value, shift)
int aligned_width = ALIGN_POWER_OF_TWO(width, 3);
buf->img.y_stride =
    ALIGN_POWER_OF_TWO((aligned_width + 2 * buf->img.border), 5);
```
