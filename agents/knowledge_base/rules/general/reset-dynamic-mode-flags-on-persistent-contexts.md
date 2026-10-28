---
name: libvpx/reset-dynamic-mode-flags-on-persistent-contexts
description: Explicitly reset or overwrite per-frame and dynamic mode flags on persistent context structs at common setup rather than conditionally setting or bitwise-accumulating
rubric: correctness
severity: should_fix
---

**Rule.** When dynamically evaluating, configuring, or updating per-frame,
per-layer, or mode flags (such as `frame_flags`, `FRAMEFLAGS_KEY`,
`simulcast_mode`, `refresh_golden_frame`, or `refresh_alt_ref_frame`) on
persistent encoder or rate control structs (such as `VP9_COMP`, `VP9_SVC`,
`VP9_COMMON`, or `RateControlRTC`), explicitly overwrite or reset the flag for
each frame or pass. Initialize or assign flags at the top-level common setup
entry block (e.g., alongside `frame_type` in `ComputeQP`) via direct
boolean/ternary assignment (e.g., `cpi->frame_flags = condition ? FLAG : 0`) or
an explicit `else` branch. Do not conditionally set flags (`flag = 1`) or
bitwise-accumulate them (`flags |= FLAG`) without clearing them on inter-frame
or negative branches, and do not bury per-frame flag initialization inside
mode-specific branches (such as single-layer vs. SVC paths).

**Why.** Encoder and rate control context structures persist across multiple
frames, layers, and encoding passes. Conditionally setting flags, using
bitwise-OR (`|=`) without clearing on inter frames, or scattering flag
initializations across conditional mode branches allows stale flag states (such
as `FRAMEFLAGS_KEY` or `simulcast_mode`) to linger and leak across subsequent
frames or layers when operating conditions change, corrupting rate control and
encoder state.

## Examples

Bad:

```cpp
// Bad Variant 1: Conditionally accumulating per-frame flags inside mode branches without resetting:
cm->frame_type = static_cast<FRAME_TYPE>(frame_params.frame_type);
cpi_->refresh_golden_frame = (cm->frame_type == KEY_FRAME) ? 1 : 0;
if (cpi_->svc.number_spatial_layers == 1) {
  // Single-layer handling...
} else {
  // Stale FRAMEFLAGS_KEY persists on subsequent inter frames:
  if (cm->frame_type == KEY_FRAME) cpi_->frame_flags |= FRAMEFLAGS_KEY;
  vp9_rc_get_svc_params(cpi_);
}

// Bad Variant 2: Enabling dynamic mode flag without clearing on negative branch:
if (svc->disable_inter_layer_pred == INTER_LAYER_PRED_OFF &&
    svc->number_spatial_layers <= 3 && svc->number_temporal_layers <= 3 &&
    !(svc->temporal_layering_mode == VP9E_TEMPORAL_LAYERING_MODE_BYPASS &&
      svc->use_set_ref_frame_config)) {
  svc->simulcast_mode = 1;
}
```

Good:

```cpp
// Good Variant 1: Explicitly initialize and overwrite per-frame flags at common entry:
cm->frame_type = static_cast<FRAME_TYPE>(frame_params.frame_type);
cpi_->frame_flags = (cm->frame_type == KEY_FRAME) ? FRAMEFLAGS_KEY : 0;
cpi_->refresh_golden_frame = (cm->frame_type == KEY_FRAME) ? 1 : 0;
if (cpi_->svc.number_spatial_layers == 1) {
  // Single-layer handling...
} else {
  vp9_rc_get_svc_params(cpi_);
}

// Good Variant 2: Explicitly reset mode flag in else branch or direct assignment:
if (svc->disable_inter_layer_pred == INTER_LAYER_PRED_OFF &&
    svc->number_spatial_layers <= 3 && svc->number_temporal_layers <= 3 &&
    !(svc->temporal_layering_mode == VP9E_TEMPORAL_LAYERING_MODE_BYPASS &&
      svc->use_set_ref_frame_config)) {
  svc->simulcast_mode = 1;
} else {
  svc->simulcast_mode = 0;
}
```
