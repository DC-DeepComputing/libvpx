---
name: libvpx/vp9/configure-speed-heuristics-in-speed-features
description: When setting VP9 speed features, configure cpi->sf across framesize-independent and dependent setters without stateful initialization, and check cpi->sf in algorithms
rubric: design
severity: should_fix
---

**Rule.** Do not branch directly on encoder speed settings (such as
cpi->oxcf.speed >= N) in mode selection or motion search routines; configure
parameters in SPEED_FEATURES (cpi->sf) and check cpi->sf instead. Within
vp9_speed_features.c, place frame-size-invariant settings in
vp9_set_speed_features_framesize_independent() and resolution thresholds in
vp9_set_speed_features_framesize_dependent(). Keep speed feature functions
purely declarative without executing state initialization helpers (such as
vp9_init3smotion_compensation), centralizing subsystem reinitialization (such as
init_motion_estimation) in the encoder loop after speed features are finalized.

**Why.** Centralizing speed features in cpi->sf avoids scattering rigid speed
branches across algorithms, while separating frame-invariant from
resolution-dependent settings avoids redundant recalculation. Keeping speed
feature setters free of stateful side effects ensures that dynamic
frame-by-frame speed transitions do not leave subsystem state stale or
uninitialized.

## Examples

Bad:

```cpp
// Anti-pattern 1: Direct speed checks in encoding routines
if (cpi->oxcf.speed >= 9 &&
    (tmp_mv->as_mv.row == 0 && tmp_mv->as_mv.col == 0)) {
  subpel_force_stop = 1;
}

// Anti-pattern 2: Initializing frame-invariant settings in framesize_dependent helper
void vp9_set_speed_features_framesize_dependent(VP9_COMP *cpi, int speed) {
  if (cpi->oxcf.motion_vector_unit_test == 1)
    cpi->find_fractional_mv_step = vp9_return_max_sub_pixel_mv;
}

// Anti-pattern 3: Calling state initialization inside speed feature setters
if (cpi->oxcf.content == VP9E_CONTENT_SCREEN && cpi->oxcf.speed > 5) {
  sf->mv.search_method = NSTEP;
  vp9_init3smotion_compensation(&cpi->ss_cfg, cpi->scaled_source.y_stride);
}
```

Good:

```cpp
// Pattern 1: Configure in vp9_speed_features.c and check cpi->sf in algorithms
if (cpi->sf.mv.enable_adaptive_subpel_force_stop) {
  subpel_force_stop = cpi->sf.mv.adapt_subpel_force_stop.force_stop_above;
}

// Pattern 2: Place frame-invariant settings in framesize_independent helper
void vp9_set_speed_features_framesize_independent(VP9_COMP *cpi, int speed) {
  if (cpi->oxcf.motion_vector_unit_test == 1)
    cpi->find_fractional_mv_step = vp9_return_max_sub_pixel_mv;
}

// Pattern 3: Set sf purely declaratively; reinitialize state in encoder loop
if (cpi->oxcf.content == VP9E_CONTENT_SCREEN && speed > 5) {
  sf->mv.search_method = NSTEP;
}
// In vp9_encoder.c after speed feature evaluation:
init_motion_estimation(cpi);
```
