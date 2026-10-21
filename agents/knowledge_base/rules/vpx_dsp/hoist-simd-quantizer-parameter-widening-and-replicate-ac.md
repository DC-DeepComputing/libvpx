---
name: libvpx/vpx_dsp/hoist-simd-quantizer-parameter-widening-and-replicate-ac
rubric: safety
severity: should_fix
---

**Rule.** In SIMD quantization implementations, hoist quantizer parameter
loading and vector widening outside the inner processing loop. Because quantizer
tables define a distinct DC value at index 0 and uniform AC values for
subsequent indices, process the initial DC chunk first and duplicate the AC lane
(such as with vdupq_lane_s32 or vdupq_lane_s16) across vector registers for all
remaining iterations instead of reloading or widening parameters per iteration.

**Why.** Quantizer parameter tables (zbin, round, quant, quant_shift, dequant)
contain identical values for all AC coefficients (index >= 1). Hoisting type
widening (e.g., vmovl_s16) outside the loop and broadcasting the AC value via
lane duplication eliminates redundant memory loads and vector unpacking
operations inside performance-critical inner loops.

## Examples

Bad:

```cpp
// Inside inner quantization loop:
for (i = 0; i < count; i += 8) {
  dqcoeff_0 = vmulq_s32(qcoeff_0, vmovl_s16(vget_low_s16(dequant)));
  dqcoeff_1 = vmulq_s32(qcoeff_1, vmovl_s16(vget_high_s16(dequant)));
  // ...
}
```

Good:

```cpp
// Hoist widening outside loop:
int32x4_t dequant = vmovl_s16(vld1_s16(dequant_ptr));
// Process first chunk (DC + 3 AC elements) and duplicate AC lane:
dqcoeff_0 = vmulq_s32(qcoeff_0, dequant);
dqcoeff_1 = vmulq_s32(qcoeff_1, vdupq_lane_s32(vget_low_s32(dequant), 1));
dequant = vdupq_lane_s32(vget_low_s32(dequant), 1);
// Subsequent iterations reuse replicated AC vector:
for (i = 8; i < count; i += 8) {
  dqcoeff_0 = vmulq_s32(qcoeff_0, dequant);
  dqcoeff_1 = vmulq_s32(qcoeff_1, dequant);
}
```
