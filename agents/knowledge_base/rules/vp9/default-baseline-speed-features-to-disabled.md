---
name: libvpx/vp9/default-baseline-speed-features-to-disabled
description: When initializing baseline encoder speed features, default optimization heuristics and pruning levels to disabled (0) before applying speed-dependent overrides so speed 0 retains maximum quality
rubric: correctness
severity: should_fix
---

**Rule.** In encoder speed feature initialization routines, ensure baseline
speed feature fields and heuristic thresholds default to disabled (0 or
exhaustive evaluation) for baseline speed 0 (highest quality). Only enable speed
optimizations, pruning heuristics, or relaxed search thresholds within
conditional speed checks (such as if (speed >= N)) or specific resolution
thresholds.

**Why.** Baseline speed feature routines define default behaviors for the
highest-quality encoding mode (speed 0). Enabling optimization levels or
heuristic shortcuts unconditionally in baseline initialization unintentionally
degrades rate-distortion performance and compression efficiency at baseline
speed settings.

## Examples

Bad:

```cpp
// In baseline speed feature initialization (vp9_speed_features.c):
void vp9_set_speed_features_framesize_independent(VP9_COMP *cpi, int speed) {
  SPEED_FEATURES *const sf = &cpi->sf;
  // Bad: enables pruning optimization unconditionally for baseline speed 0
  sf->cb_pred_filter_search = 2;

  if (speed >= 1) {
    sf->disable_split_mask = DISABLE_COMPOUND_SPLIT;
  }
}
```

Good:

```cpp
// In baseline speed feature initialization (vp9_speed_features.c):
void vp9_set_speed_features_framesize_independent(VP9_COMP *cpi, int speed) {
  SPEED_FEATURES *const sf = &cpi->sf;
  // Good: defaults to disabled (0) for baseline speed 0 (highest quality)
  sf->cb_pred_filter_search = 0;

  if (speed >= 1) {
    sf->cb_pred_filter_search = 2;
    sf->disable_split_mask = DISABLE_COMPOUND_SPLIT;
  }
}
```
