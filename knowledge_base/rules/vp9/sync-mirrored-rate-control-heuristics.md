---
name: libvpx/vp9/sync-mirrored-rate-control-heuristics
description: When modifying rate-control heuristics, bandwidth allocations, or rounding calculations in VP9 base or SVC code, synchronize counterpart logic across both modes
rubric: correctness
severity: should_fix
---

**Rule.** When modifying rate-control algorithms, bandwidth allocations, target
frame sizes, reset heuristics, threshold comparisons, or rounding and clamping
logic (such as avg_frame_bandwidth = round(target_bandwidth / framerate),
saturate_cast_double_to_int, or check_reset_rc_flag heuristics) in
vp9_ratectrl.c or vp9_encoder.c, update all counterpart calculations in
vp9_svc_layercontext.c (and vice versa). Ensure per-layer rate-control
structures (LAYER_CONTEXT, lc->rc, lrc) mirror base rate-control formulas
(cpi->rc) across all spatial and temporal layers.

**Why.** SVC layer context maintains independent rate-control state (lrc /
lc->rc) for each spatial and temporal layer that mirrors base single-layer rate
control (cpi->rc). Modifying rate-control reset heuristics, threshold checks,
bandwidth calculations, or rounding in one path without updating the other
leaves the counterpart path vulnerable to integer overflow, mathematical drift,
stale reset states, or behavioral inconsistencies between single-layer and
multi-layer SVC modes.

## Examples

Bad:

```cpp
// Variant 1 (heuristics): updated in SVC, but missed in non-SVC counterpart
// In vp9_svc_layercontext.c:
if (lrc->avg_frame_bandwidth / 3 > (lrc->last_avg_frame_bandwidth >> 1) ||
    lrc->avg_frame_bandwidth < (lrc->last_avg_frame_bandwidth >> 1)) {
  lrc->rc_1_frame = 0;
  lrc->rc_2_frame = 0;
}
// In vp9_encoder.c (missed, susceptible to overflow):
if (rc->avg_frame_bandwidth > (3 * rc->last_avg_frame_bandwidth >> 1) ||
    rc->avg_frame_bandwidth < (rc->last_avg_frame_bandwidth >> 1)) {
  rc->rc_1_frame = 0;
  rc->rc_2_frame = 0;
}

// Variant 2 (calculations): rounded in base RC, but truncated in SVC
// In vp9_ratectrl.c:
rc->avg_frame_bandwidth = saturate_cast_double_to_int(
    round(target_bandwidth / framerate));
// In vp9_svc_layercontext.c:
lrc->avg_frame_bandwidth =
    (int)VPXMIN(lc->target_bandwidth / lc->framerate, INT_MAX);
```

Good:

```cpp
// Variant 1 (heuristics): synchronized across SVC and non-SVC paths
// In vp9_svc_layercontext.c:
if (lrc->avg_frame_bandwidth / 3 > (lrc->last_avg_frame_bandwidth >> 1) ||
    lrc->avg_frame_bandwidth < (lrc->last_avg_frame_bandwidth >> 1)) {
  lrc->rc_1_frame = 0;
  lrc->rc_2_frame = 0;
}
// In vp9_encoder.c:
if (rc->avg_frame_bandwidth / 3 > (rc->last_avg_frame_bandwidth >> 1) ||
    rc->avg_frame_bandwidth < (rc->last_avg_frame_bandwidth >> 1)) {
  rc->rc_1_frame = 0;
  rc->rc_2_frame = 0;
}

// Variant 2 (calculations): consistent rounding in base and SVC contexts
// In vp9_ratectrl.c:
rc->avg_frame_bandwidth = saturate_cast_double_to_int(
    round(target_bandwidth / framerate));
// In vp9_svc_layercontext.c:
lrc->avg_frame_bandwidth = saturate_cast_double_to_int(
    round(lc->target_bandwidth / lc->framerate));
```
