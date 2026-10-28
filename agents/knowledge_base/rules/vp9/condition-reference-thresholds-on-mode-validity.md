---
name: libvpx/vp9/condition-reference-thresholds-on-mode-validity
description: When conditionally validating or disabling an encoding or reference frame mode, guard mode-specific threshold calculations so they only execute when the mode remains active
rubric: correctness
severity: should_fix
---

**Rule.** When validating preconditions for a reference frame mode (such as
checking scale factor compatibility for temporal reference frames) and disabling
the mode on failure, place mode-specific threshold calculations inside the
enabled branch. Do not compute or overwrite mode-specific heuristic thresholds
unconditionally when the mode has been disabled.

**Why.** Executing mode-specific threshold overrides unconditionally after a
mode has failed validation clobbers default thresholds or settings configured
for alternative modes (such as spatial inter-layer references), causing
incorrect mode evaluation and rate-distortion regressions.

## Examples

Bad:

```cpp
if ((sf_last->x_scale_fp != sf_golden->x_scale_fp) ||
    (sf_last->y_scale_fp != sf_golden->y_scale_fp)) {
  gf_temporal_ref = 0;
}
// Bad: Unconditionally overrides thresholds even when mode was disabled
if (cpi->rc.avg_frame_low_motion > 70) {
  thresh_svc_skip_golden = 500;
} else {
  thresh_svc_skip_golden = 0;
}
```

Good:

```cpp
if ((sf_last->x_scale_fp != sf_golden->x_scale_fp) ||
    (sf_last->y_scale_fp != sf_golden->y_scale_fp)) {
  gf_temporal_ref = 0;
} else {
  // Good: Guard mode-specific threshold calculations so they apply only when enabled
  if (cpi->rc.avg_frame_low_motion > 70) {
    thresh_svc_skip_golden = 500;
  } else {
    thresh_svc_skip_golden = 0;
  }
}
```
