---
name: libvpx/vp9/update-cyclic-refresh-parameters-before-qp-selection
description: When computing rate control parameters or frame QP with VP9 cyclic refresh adaptive quantization (`CYCLIC_REFRESH_AQ`), invoke `vp9_cyclic_refresh_update_parameters(cpi)` before selecting QP and target bitrate
rubric: correctness
severity: should_fix
---

**Rule.** When configuring rate control or computing frame QP and target
bitrates in VP9 with cyclic refresh adaptive quantization enabled (`oxcf.aq_mode
== CYCLIC_REFRESH_AQ`), invoke `vp9_cyclic_refresh_update_parameters(cpi)`
before calculating frame QP or target sizes. Do not select quantizer bounds or
target bitrates without first refreshing cyclic refresh delta-QP and macroblock
budget state.

**Why.** Cyclic refresh adjusts delta-QP offsets, background macroblock refresh
counts, and rate boosts per frame. If parameters are not updated prior to rate
control bit allocation and QP selection, the rate controller computes target
bitrates and active QP bounds using stale quantization state.

## Examples

Bad:

```cpp
cpi_->sf.use_nonrd_pick_mode = 1;
if (cpi_->svc.number_spatial_layers == 1 &&
    cpi_->svc.number_temporal_layers == 1) {
  int target = 0;
  // Computing QP and bit target without updating cyclic refresh state
  target = vp9_calc_pframe_target_size_one_pass_cbr(cpi_);
  cpi_->rc.base_frame_target = target;
}
```

Good:

```cpp
cpi_->sf.use_nonrd_pick_mode = 1;
if (cpi_->oxcf.aq_mode == CYCLIC_REFRESH_AQ) {
  vp9_cyclic_refresh_update_parameters(cpi_);
}
if (cpi_->svc.number_spatial_layers == 1 &&
    cpi_->svc.number_temporal_layers == 1) {
  int target = 0;
  target = vp9_calc_pframe_target_size_one_pass_cbr(cpi_);
  cpi_->rc.base_frame_target = target;
}
```
