---
name: libvpx/vp8/omit-redundant-mode-checks-in-specialized-rtc-ratectrl
description: When adapting rate control logic into dedicated RTC rate controllers, omit redundant mode checks, non-RTC GF/ARF min-q tables, and multi-layer or reference-refresh conditions
rubric: readability
severity: should_fix
---

**Rule.** When implementing or adapting rate control routines into dedicated
real-time rate controllers (such as `VP8RateControlRTC` or `VP9RateControlRTC`),
omit non-RTC rate control paths, redundant mode checks, and unused frame-type
branching carried over from full encoder implementations:
1. Omit conditional checks for usage modes (such as `end_usage == USAGE_STREAM_FROM_SERVER`) that are fixed by design in RTC mode.
2. Omit golden or alt-ref frame quality boost adjustments (such as `gf_high_motion_minq` tables and branches), using standard inter-frame bounds (`inter_minq`) for all non-key frames.
3. When updating normal inter-frame statistics (such as `ni_frames` and `ni_tot_qi`), check only `cm->frame_type != KEY_FRAME` rather than checking `number_of_layers` or `refresh_golden_frame`/`refresh_alt_ref_frame` flags, and keep accompanying comments synchronized.

**Why.** Specialized RTC rate controllers operate strictly under real-time
constraints (such as CBR) where operating modes are fixed at initialization, and
non-key frames do not use separate golden/alt-ref quality boost curves or
multi-layer reference exclusions. Retaining unused general-encoder branches,
lookup tables (`gf_high_motion_minq`), or multi-layer/reference-refresh checks
adds dead or redundant code and causes explanatory comments to drift from actual
RTC rate control behavior.

## Examples

Bad:

```cpp
// Retaining redundant mode checks and non-RTC GF/ARF quality boost branches:
if (cpi_->oxcf.end_usage == USAGE_STREAM_FROM_SERVER) {
  vp8_pick_frame_size(cpi_);
}
if (cm->frame_type == KEY_FRAME) {
  cpi_->active_best_quality = kf_high_motion_minq[q];
} else if (cpi_->oxcf.number_of_layers == 1 &&
           (cm->refresh_golden_frame || cpi_->common.refresh_alt_ref_frame)) {
  cpi_->active_best_quality = gf_high_motion_minq[q];
} else {
  cpi_->active_best_quality = inter_minq[q];
}

// Retaining redundant layer count and reference refresh checks for ni_frames:
/* Keep a record from which we can calculate average Q excluding GF/key frames */
if ((cm->frame_type != KEY_FRAME) &&
    ((cpi_->oxcf.number_of_layers > 1) ||
     (!cm->refresh_golden_frame && !cm->refresh_alt_ref_frame))) {
  cpi_->ni_frames++;
  cpi_->ni_tot_qi += q_;
}
```

Good:

```cpp
// Execute fixed CBR routines directly and treat all non-key frames with standard inter bounds:
vp8_pick_frame_size(cpi_);
if (cm->frame_type == KEY_FRAME) {
  cpi_->active_best_quality = kf_high_motion_minq[q];
} else {
  cpi_->active_best_quality = inter_minq[q];
}

// Update normal inter statistics for all non-key frames with accurate comment:
/* Keep a record from which we can calculate the average Q excluding key frames. */
if (cm->frame_type != KEY_FRAME) {
  cpi_->ni_frames++;
  cpi_->ni_tot_qi += q_;
}
```
