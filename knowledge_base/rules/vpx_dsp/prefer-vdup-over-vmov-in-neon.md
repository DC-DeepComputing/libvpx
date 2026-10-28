---
name: libvpx/vpx_dsp/prefer-vdup-over-vmov-in-neon
description: When broadcasting or duplicating scalar values into ARM NEON vectors, use standard vdup_n_* or vdupq_n_* intrinsics rather than non-standard vmov_n_* or vmovq_n_*
rubric: readability
severity: nit
---

**Rule.** In ARM NEON intrinsics code, use standard `vdup_n_*` and `vdupq_n_*`
intrinsics rather than `vmov_n_*` or `vmovq_n_*` to duplicate or broadcast
scalar values across vector lanes.

**Why.** `vdup_n_*` and `vdupq_n_*` are the standard ARM NEON vector duplicate
intrinsics used across libvpx, whereas `vmov_n_*` and `vmovq_n_*` are
non-standard vendor aliases that introduce inconsistency across SIMD kernels.

## Examples

Bad:

```cpp
const int16x8_t max = vmovq_n_s16((1 << bd) - 1);
const uint8x8_t zero = vmov_n_u8(0);
```

Good:

```cpp
const int16x8_t max = vdupq_n_s16((1 << bd) - 1);
const uint8x8_t zero = vdup_n_u8(0);
```
