---
name: libvpx/vp9/configure-reference-refresh-flags-early
description: When configuring encoder reference refresh flags (refresh_alt_ref_frame, alt_fb_idx), set them during early frame setup or state resets rather than in late-stage denoiser update routines
rubric: correctness
severity: should_fix
---

**Rule.** Do not configure or modify encoder reference frame refresh flags (such
as cpi->refresh_alt_ref_frame, cpi->refresh_golden_frame, or buffer slot indices
like cpi->alt_fb_idx) inside late-stage post-encode update helpers like
vp9_denoiser_update_ref_frame. Configure reference refresh flags and buffer
indices during early frame setup or when resetting state (e.g., when
denoiser.reset = 1 in vp9_denoiser_reset_on_first_frame).

**Why.** In VP9 encoding, reference frame update decisions and header flags are
evaluated before or during frame encoding. Modifying reference refresh flags in
post-encode update helpers executes after the encoder has already processed
reference state, causing reference buffers to desynchronize between the encoder
and the denoiser long-term reference tracking.

## Examples

Bad:

```cpp
void vp9_denoiser_update_ref_frame(VP9_COMP *const cpi) {
  // Anti-pattern: configuring encoder reference refresh flags late
  // in post-encode denoiser update routine.
  if (cpi->use_svc) {
    cpi->alt_fb_idx = cpi->svc.buffer_gf_temporal_ref[index].idx;
    cpi->refresh_alt_ref_frame = 1;
  }
  ...
}
```

Good:

```cpp
void vp9_denoiser_reset_on_first_frame(VP9_COMP *const cpi) {
  // Good: configure reference refresh flags early during state reset
  if (vp9_denoise_svc_non_key(cpi) &&
      cpi->denoiser.current_denoiser_frame == 0) {
    cpi->denoiser.reset = 1;
    force_refresh_longterm_ref(cpi);
  }
}

void vp9_denoiser_update_ref_frame(VP9_COMP *const cpi) {
  // Only update denoiser internal buffers using active encoder flags
  ...
}
```
