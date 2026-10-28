---
name: libvpx/clean-up-dead-stores-when-removing-output-metrics
description: When refactoring logic, algorithms, or loops, remove obsolete local variables, dead stores, redundant prerequisite setup calls, and superseded special-case assignments
rubric: readability
severity: should_fix
---

**Rule.** When refactoring control flow, updating algorithms, generalizing logic
into loops, or removing diagnostic outputs, clean up all obsolete code paths and
orphaned artifacts:
1. Delete unused local variable declarations, dead stores, obsolete threshold constants, and unconsumed intermediate calculations, including within conditionally compiled (#ifdef) blocks.
2. Remove obsolete prerequisite setup and helper calls (such as vp9_set_mb_mi or initialization routines) that were only needed by superseded conditions and are handled downstream.
3. Remove superseded special-case assignments and branches (such as individual reference frame setup like ref_frame_sign_bias) that are now covered or overwritten by generalized loops or helpers.

**Why.** Leaving obsolete variables, dead calculations, redundant setup calls,
or superseded special-case logic clutters the codebase, wastes CPU cycles, and
misleads readers about what parameters and initializations actually govern
runtime behavior. In conditionally compiled blocks or alternate build
configurations, dead variables and obsolete code can mask incomplete
refactoring.

## Examples

Bad:

```cpp
// 1. Dead calculations / obsolete thresholds left after refactoring:
const double target_rate = (double)cpi->oxcf.target_bandwidth / 1000;
const double rate_err = ((100.0 * (dr - target_rate)) / target_rate);
unsigned int thresh_spatial_var = (32 * 32) << 8;

// 2. Obsolete setup call retained before refactored condition:
vp9_set_mb_mi(cm, cm->width, cm->height);
if (cpi->initial_width && cm->width > cpi->initial_width) {
  cpi->initial_width = cm->width;
}

// 3. Superseded special-case assignment before generalized loop:
cm->ref_frame_sign_bias[ALTREF_FRAME] = 1;
for (ref_frame = LAST_FRAME; ref_frame < MAX_REF_FRAMES; ++ref_frame) {
  cm->ref_frame_sign_bias[ref_frame] = get_sign_bias(cpi, ref_frame);
}
```

Good:

```cpp
// 1. Dead calculations and unused thresholds removed:
// (target_rate, rate_err, and thresh_spatial_var deleted)

// 2. Obsolete prerequisite setup call removed:
if (cpi->initial_width && cm->width > cpi->initial_width) {
  cpi->initial_width = cm->width;
}

// 3. Generalized loop handles all frames without redundant special-case setup:
for (ref_frame = LAST_FRAME; ref_frame < MAX_REF_FRAMES; ++ref_frame) {
  cm->ref_frame_sign_bias[ref_frame] = get_sign_bias(cpi, ref_frame);
}
```
