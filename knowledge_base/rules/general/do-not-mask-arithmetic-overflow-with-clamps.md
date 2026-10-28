---
name: libvpx/do-not-mask-arithmetic-overflow-with-clamps
description: When handling integer arithmetic helper outputs, do not mask wrap-around or overflow with downstream ad-hoc clamps; fix the calculation or input contract
rubric: correctness
severity: should_fix
---

**Rule.** Do not add downstream ad-hoc clamping or wrap-around checks (such as
checking if an unsigned or squared metric turned negative and clamping to
INT64_MAX) to mask integer arithmetic overflows. Treat arithmetic overflow in
helper functions as an algorithm or contract bug, and resolve it by ensuring
caller inputs conform to expected bit-depth contracts or upgrading intermediate
calculation types.

**Why.** Even though unsigned integer overflow is defined behavior in C,
wrapping arithmetic produces incorrect rate-distortion and metric computations.
Adding downstream ad-hoc clamping workarounds masks underlying arithmetic bugs
or invalid out-of-range inputs that violate API contracts.

## Examples

Bad:

```cpp
sse = (int64_t)vpx_sum_squares_2d_i16(diff, diff_stride, tx_4x4_w << 2);
// Downstream clamp masking wrap-around overflow from out-of-range inputs
if (sse < 0) sse = INT64_MAX;
```

Good:

```cpp
/* Ensure inputs conform to bit-depth contract rather than masking overflow */
sse = (int64_t)vpx_sum_squares_2d_i16(diff, diff_stride, tx_4x4_w << 2);
```
