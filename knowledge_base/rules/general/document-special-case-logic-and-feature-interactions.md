---
name: libvpx/document-special-case-logic-and-feature-interactions
description: When implementing special-case branches, compound predicates, heuristic thresholds, or feature interactions, add explanatory comments detailing intent, rationale, and invariants
rubric: readability
severity: nit
---

**Rule.** When introducing special-case control flow, non-obvious compound
conditions or multi-clause predicates (such as detecting no-op configurations,
default parameter combinations, or early returns), heuristic thresholds, interim
policies, hardcoded simplifying assumptions, state updates enforcing invariants
(such as resolution or buffer clamping), or interactions between multiple
encoder features (such as ROI maps, cyclic refresh, and segmentation), accompany
the logic with an explanatory comment detailing the high-level intent,
rationale, and underlying constraints. Ensure comments explain why the handling
or exception is necessary rather than restating syntax, and use descriptive
named constants for thresholds and frame-delay counts instead of bare magic
numbers.

**Why.** Complex multi-clause conditions, heuristic thresholds, interacting
subsystems (such as ROI, cyclic refresh, and segmentation), and simplifying
assumptions lack self-evident context in code. Without inline comments
explaining the rationale, invariants, and failure modes being prevented,
non-obvious branch conditions and state updates are opaque to reviewers and risk
accidental regressions or incorrect assumptions during refactoring.

## Examples

Bad:

```cpp
// Non-obvious compound condition and early return:
if (!map || (delta_q[0] == 0 && delta_q[1] == 0 && delta_q[2] == 0 &&
             delta_q[3] == 0 && delta_lf[0] == 0 && delta_lf[1] == 0 &&
             delta_lf[2] == 0 && delta_lf[3] == 0 && threshold[0] == 0 &&
             threshold[1] == 0 && threshold[2] == 0 && threshold[3] == 0)) {
  disable_segmentation(cpi);
  return 0;
}

// Non-obvious heuristic condition without context:
int64_t new_buffer_level =
    cpi->rc.buffer_level + cpi->rc.avg_frame_bandwidth - frame_size;
if (new_buffer_level < 0) return 1;

// Feature interaction and magic numbers without explanation:
if (cpi->roi.enabled && !frame_is_intra_only(cm)) {
  if (cpi->roi.skip[3] && cpi->rc.frames_since_key > 20) {
    if (cpi->oxcf.aq_mode == CYCLIC_REFRESH_AQ) vp9_cyclic_refresh_setup(cpi);
  }
}
```

Good:

```cpp
// Explains compound condition detecting no-op configuration:
// Disable segmentation if no map is provided or all deltas and thresholds are zero.
if (!map || (delta_q[0] == 0 && delta_q[1] == 0 && delta_q[2] == 0 &&
             delta_q[3] == 0 && delta_lf[0] == 0 && delta_lf[1] == 0 &&
             delta_lf[2] == 0 && delta_lf[3] == 0 && threshold[0] == 0 &&
             threshold[1] == 0 && threshold[2] == 0 && threshold[3] == 0)) {
  disable_segmentation(cpi);
  return 0;
}

// Explaining heuristic drop policy:
int64_t new_buffer_level =
    cpi->rc.buffer_level + cpi->rc.avg_frame_bandwidth - frame_size;
// Drop the frame if the predicted buffer level after encoding goes below zero.
if (new_buffer_level < 0) return 1;

// Explains feature interaction and uses descriptive named constants:
// If ROI is enabled and background skip is used, allow cyclic refresh after keyframe delay.
if (cpi->roi.enabled && !frame_is_intra_only(cm)) {
  if (cpi->roi.skip[BACKGROUND_SEG_SKIP_ID] &&
      cpi->rc.frames_since_key > FRAMES_NO_SKIPPING_AFTER_KEY) {
    if (cpi->oxcf.aq_mode == CYCLIC_REFRESH_AQ) vp9_cyclic_refresh_setup(cpi);
  }
}
```
