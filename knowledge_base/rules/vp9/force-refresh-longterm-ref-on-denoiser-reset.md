---
name: libvpx/vp9/force-refresh-longterm-ref-on-denoiser-reset
description: When resetting the VP9 temporal denoiser state (denoiser->reset), invoke force_refresh_longterm_ref() to keep long-term reference frame buffers synchronized across SVC layers
rubric: correctness
severity: should_fix
---

**Rule.** When resetting the VP9 temporal denoiser state (setting
cpi->denoiser.reset = 1 or denoiser->reset = 1), invoke
force_refresh_longterm_ref(cpi) to trigger an alt-ref frame buffer update.
Ensure long-term temporal reference buffers remain synchronized with the
denoiser when switching noise levels or resetting on the first frame of an SVC
layer.

**Why.** Resetting the temporal denoiser invalidates running average history.
Without refreshing long-term reference buffers via force_refresh_longterm_ref(),
reference frame buffers drift out of sync with the denoiser state across SVC
temporal layers, degrading video quality.

## Examples

Bad:

```cpp
void vp9_denoiser_reset_on_first_frame(VP9_COMP *const cpi) {
  if (denoise_svc_pickmode && cpi->denoiser.current_denoiser_frame == 0) {
    cpi->denoiser.reset = 1;
  }
}
```

Good:

```cpp
void vp9_denoiser_reset_on_first_frame(VP9_COMP *const cpi) {
  if (denoise_svc_pickmode && cpi->denoiser.current_denoiser_frame == 0) {
    cpi->denoiser.reset = 1;
    force_refresh_longterm_ref(cpi);
  }
}
```
