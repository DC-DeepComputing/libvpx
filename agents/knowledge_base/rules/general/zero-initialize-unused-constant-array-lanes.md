---
name: libvpx/zero-initialize-unused-constant-array-lanes
description: When defining constant lookup tables, aligned arrays, or SIMD vector constants with padding lanes, initialize unused slots to 0 rather than non-zero dummy values
rubric: readability
severity: nit
---

**Rule.** Initialize unused or padding elements in constant lookup tables,
aligned arrays, and SIMD vector constants to 0 rather than arbitrary non-zero
dummy values. Explicitly zero-initializing unused slots clarifies that they are
unreferenced padding and prevents readers from mistaking dummy values for active
coefficients.

**Why.** Non-zero dummy values in unreferenced vector lanes or array padding can
mislead readers into assuming those values are required or accessed by
mathematical algorithms. Explicitly setting unused elements to 0 clarifies data
flow and makes padding immediately identifiable.

## Examples

Bad:

```cpp
DECLARE_ALIGNED(16, static const int16_t, cospi[4]) = {
  16384, (int16_t)cospi_8_64, (int16_t)cospi_16_64, (int16_t)cospi_24_64
};
```

Good:

```cpp
DECLARE_ALIGNED(16, static const int16_t, cospi[4]) = {
  0, (int16_t)cospi_8_64, (int16_t)cospi_16_64, (int16_t)cospi_24_64
};
```
