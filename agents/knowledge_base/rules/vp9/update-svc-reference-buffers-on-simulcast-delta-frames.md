---
name: libvpx/vp9/update-svc-reference-buffers-on-simulcast-delta-frames
description: When updating reference frame buffers in VP9 1-pass CBR SVC, do not bypass vp9_svc_update_ref_frame on delta frames in simulcast mode
rubric: correctness
severity: should_fix
---

**Rule.** In VP9 1-pass CBR SVC mode, do not bypass vp9_svc_update_ref_frame()
when simulcast_mode is enabled. Ensure reference frame buffer update routines
execute on delta frames so frame buffer slot mappings and layer tracking IDs
(fb_idx_spatial_layer_id, fb_idx_temporal_layer_id) remain synchronized across
layers.

**Why.** In simulcast mode, each spatial layer encodes without inter-layer
prediction but still relies on intra-layer temporal reference frames. Bypassing
vp9_svc_update_ref_frame() prevents delta frames from updating reference buffer
slot indices and layer IDs, leading to corrupted reference buffer tracking and
broken inter-frame prediction.

## Examples

Bad:

```cpp
// Bypassing SVC reference update in simulcast mode breaks delta frame tracking
if (!cpi->svc.simulcast_mode && is_one_pass_cbr_svc(cpi)) {
  vp9_svc_update_ref_frame(cpi);
}
```

Good:

```cpp
// Always run reference frame updates for 1-pass CBR SVC so delta frames track buffer slots
if (is_one_pass_cbr_svc(cpi)) {
  vp9_svc_update_ref_frame(cpi);
}
```
