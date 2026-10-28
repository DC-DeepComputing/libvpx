---
name: libvpx/bitwise-power-of-two-alignment
description: When rounding up or aligning an integer to a power-of-two boundary N, use `(x + (N - 1)) & ~(N - 1)` instead of custom subtraction or modulo expressions
rubric: correctness
severity: nit
---

**Rule.** When rounding up or aligning an integer or dimension to a power-of-two
boundary N, use the standard bitwise idiom '(x + (N - 1)) & ~(N - 1)' (or '(x +
mask) & ~mask' where mask = N - 1). Do not use custom subtraction or modulo
expressions such as 'x + (N - (x & (N - 1)))' or 'x + (N - x % N)', which
miscalculate and add N when the input is already an exact multiple of N.

**Why.** Expressions like 'x + (N - (x & (N - 1)))' or 'x + (N - x % N)'
evaluate to x + N when x is already aligned, producing incorrect buffer
dimensions, strides, or loop bounds. The bitwise idiom '(x + (N - 1)) & ~(N -
1)' correctly and branchlessly rounds up to the nearest multiple of N for all
non-negative inputs.

## Examples

Bad:

```cpp
// Incorrectly evaluates to h + 8 when h is already a multiple of 8
int aligned_h = h + (8 - (h & 0x7));
int aligned_w = w + (16 - (w % 16));
```

Good:

```cpp
// Correctly rounds up to multiple of 8 or 16 for all non-negative values
int aligned_h = (h + 7) & ~7;
int aligned_w = (w + 15) & ~15;
```
