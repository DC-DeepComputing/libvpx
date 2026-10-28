---
name: libvpx/vp9/vp9-guard-base-layer-keyframe-in-svc-simulcast
description: When implementing base-layer rate control or key frame logic in VP9 SVC, guard KEY_FRAME checks with !svc->simulcast_mode or spatial_layer_id == 0
rubric: correctness
severity: should_fix
---

**Rule.** In VP9 SVC encoding, do not assume cm->frame_type == KEY_FRAME applies
only to the base spatial layer. When executing base-layer-specific rate control
updates, state resets, or key frame adjustments, verify that the current frame
is actually the base layer by checking !svc->simulcast_mode or
svc->spatial_layer_id == 0.

**Why.** In standard VP9 SVC, only the base spatial layer (spatial layer 0) is
encoded as a KEY_FRAME while higher spatial layers are inter frames. In
simulcast mode (where inter-layer prediction is disabled), upper spatial layers
are also encoded as KEY_FRAMEs, causing unguarded KEY_FRAME branches to execute
erroneously during upper spatial layer encoding.

## Examples

Bad:

```cpp
if (cm->frame_type == KEY_FRAME && cpi->oxcf.rc_mode == VPX_CBR &&
    rc->projected_frame_size > 3 * rc->avg_frame_bandwidth) {
  // Resets base-layer average qindex assuming only layer 0 is KEY_FRAME
  vp9_zero(lrc->avg_frame_qindex);
}
```

Good:

```cpp
if (cm->frame_type == KEY_FRAME && !svc->simulcast_mode &&
    cpi->oxcf.rc_mode == VPX_CBR &&
    rc->projected_frame_size > 3 * rc->avg_frame_bandwidth) {
  // Guarded: only runs on base layer key frame
  vp9_zero(lrc->avg_frame_qindex);
}
```
