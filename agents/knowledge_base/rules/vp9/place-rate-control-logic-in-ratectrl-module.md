---
name: libvpx/vp9/place-rate-control-logic-in-ratectrl-module
description: When implementing subsystem logic or state transitions (rate control, SVC, denoiser), define helpers in subsystem modules (vp9_ratectrl, vp9_svc_layercontext, vp9_denoiser) instead of vp9_encoder.c
rubric: design
severity: should_fix
---

**Rule.** Place subsystem-specific algorithms, state transitions, and reference
buffer management—such as rate control decisions (frame dropping, buffer
updates, QP targets), SVC layer tracking, or temporal denoising—in dedicated
subsystem files (e.g., vp9_ratectrl.c, vp9_svc_layercontext.c, vp9_denoiser.c)
with public declarations in their respective headers, rather than defining or
inlining them in top-level orchestration files like vp9_encoder.c.

**Why.** Inlining subsystem logic or defining domain-specific static helpers in
vp9_encoder.c bloats the core encoder pipeline, increases coupling across
modules, violates modular separation, and makes rate control, SVC, and denoiser
logic difficult to isolate, test, and maintain independently.

## Examples

Bad:

```cpp
// In vp9/encoder/vp9_encoder.c - Inlining rate control & SVC logic directly:
static int post_encode_drop_screen_content(VP9_COMP *cpi, size_t *size) {
  if (cpi->rc.buffer_level < cpi->oxcf.drop_frames_water_mark) {
    return 1;
  }
  return 0;
}

if (is_one_pass_cbr_svc(cpi)) {
  if (cm->frame_type == KEY_FRAME) {
    for (i = 0; i < REF_FRAMES; i++) {
      svc->fb_idx_spatial_layer_id[i] = svc->spatial_layer_id;
      svc->fb_idx_temporal_layer_id[i] = svc->temporal_layer_id;
    }
  }
  vp9_copy_flags_ref_update_idx(cpi);
  vp9_svc_update_ref_frame_buffer_idx(cpi);
}
```

Good:

```cpp
// In vp9/encoder/vp9_ratectrl.c (declared in vp9_ratectrl.h):
int post_encode_drop_cbr(VP9_COMP *cpi, size_t *size) {
  if (cpi->rc.buffer_level < cpi->oxcf.drop_frames_water_mark) {
    return 1;
  }
  return 0;
}

// In vp9/encoder/vp9_svc_layercontext.c (declared in vp9_svc_layercontext.h):
void vp9_svc_update_ref_frame(VP9_COMP *const cpi) {
  // Encapsulated SVC state updates and buffer indexing logic
  vp9_svc_update_ref_frame_buffer_idx(cpi);
}

// In vp9/encoder/vp9_encoder.c - Core loop delegates to subsystem helpers:
if (post_encode_drop_cbr(cpi, size)) return;
if (is_one_pass_cbr_svc(cpi)) {
  vp9_svc_update_ref_frame(cpi);
}
```
