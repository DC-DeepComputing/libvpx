---
name: libvpx/vp9/update-frame-drop-tracking-state-flags
description: When dropping a frame in VP9 encoder routines, ensure both cpi->last_frame_dropped and SVC last_layer_dropped tracking flags are set to 1
rubric: correctness
severity: should_fix
---

**Rule.** When implementing or modifying frame-dropping logic (such as
post-encode drops, buffer overflow bailouts, or spatial layer drops), ensure
both cpi->last_frame_dropped and, when SVC is enabled (cpi->use_svc),
cpi->svc.last_layer_dropped[cpi->svc.spatial_layer_id] are set to 1 upon
dropping a frame. Do not exit a drop path without updating these tracking flags.

**Why.** Downstream rate control heuristics, speed feature selections, and SVC
layer dependency management inspect last_frame_dropped and last_layer_dropped to
determine if previous frame data is available. Leaving these flags unset after a
drop causes the encoder to make speed and reference decisions assuming the frame
was successfully coded, corrupting rate control and encoder state.

## Examples

Bad:

```cpp
if (new_buffer_level < 0) {
  *size = 0;
  vp9_rc_postencode_update_drop_frame(cpi);
  return 1;
}
```

Good:

```cpp
if (new_buffer_level < 0) {
  *size = 0;
  vp9_rc_postencode_update_drop_frame(cpi);
  cpi->last_frame_dropped = 1;
  if (cpi->use_svc) {
    cpi->svc.last_layer_dropped[cpi->svc.spatial_layer_id] = 1;
  }
  return 1;
}
```
