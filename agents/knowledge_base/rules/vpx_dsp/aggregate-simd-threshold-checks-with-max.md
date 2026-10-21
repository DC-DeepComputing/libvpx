---
name: libvpx/vpx_dsp/aggregate-simd-threshold-checks-with-max
rubric: safety
severity: should_fix
---

**Rule.** When checking whether multiple SIMD vector values are all below (or
all above) a common threshold, aggregate the vectors using vector maximum (or
vector minimum) intrinsics before performing a single threshold comparison. Do
not perform separate threshold comparisons on each vector and combine the
resulting masks with bitwise AND operations.

**Why.** Evaluating N vector threshold conditions individually requires N
comparisons plus N - 1 bitwise AND operations (2N - 1 vector instructions).
Aggregating values first with N - 1 vector max (or min) operations followed by a
single comparison reduces the total instruction count to N, lowering register
pressure and latency in DSP loop-filtering and thresholding kernels.

## Examples

Bad:

```cpp
// Comparing each vector individually and ANDing the masks (2N - 1 ops)
uint8x8_t mask = vand_u8(vclt_u8(a2, filter), vclt_u8(a1, filter));
mask = vand_u8(mask, vclt_u8(b1, filter));
mask = vand_u8(mask, vclt_u8(b2, filter));
return mask;
```

Good:

```cpp
// Aggregating with vector maximum and comparing once (N ops)
uint8x8_t max_val = vmax_u8(a2, a1);
max_val = vmax_u8(max_val, b1);
max_val = vmax_u8(max_val, b2);
return vclt_u8(max_val, filter);
```
