---
name: libvpx/vp9/guard-svc-state-with-use-svc
description: In shared VP9 encoder paths, guard SVC context state accesses (`cpi->svc`) and SVC helper routines (`vp9_svc_*`, `vp9_denoise_svc_*`) behind `cpi->use_svc`
rubric: correctness
severity: should_fix
---

**Rule.** In VP9 encoder routines shared between standard single-layer encoding
and Scalable Video Coding (SVC), always guard accesses and updates to SVC
context state (`cpi->svc`, `cpi->svc.layer_context`,
`cpi->svc.spatial_layer_id`, `cpi->svc.last_layer_dropped`) and invocations of
SVC-specific helper routines (such as `vp9_denoise_svc_non_key`, `vp9_svc_*`, or
`vp9_denoiser_reset_on_first_frame`) behind a check for `cpi->use_svc`. Do not
read or mutate `cpi->svc` layer tracking variables or invoke SVC-dependent
helpers unconditionally in shared or non-SVC code paths.

**Why.** When SVC is disabled (`cpi->use_svc == 0`), `cpi->svc` layer context
structures (`cpi->svc.layer_context`) and per-layer tracking variables are
uninitialized or contain stale metadata. Unconditionally accessing `cpi->svc`
state or invoking SVC-dependent helper functions in non-SVC mode accesses
invalid memory, corrupts rate control or denoising state, or overwrites non-SVC
encoder defaults.

## Examples

Bad:

```cpp
// Direct SVC layer state access without use_svc check
cpi->last_frame_dropped = 1;
cpi->svc.last_layer_dropped[cpi->svc.spatial_layer_id] = 1;

// Unconditional call to SVC helper in shared code path
denoise_svc_pickmode = vp9_denoise_svc_non_key(cpi);
if (cpi->denoiser.denoising_level > kDenLowLow && denoise_svc_pickmode) {
  denoise_svc(&cpi->denoiser, &cpi->svc, ...);
}

// Calling SVC-dependent reset helper without use_svc guard
#if CONFIG_VP9_TEMPORAL_DENOISING
if (cpi->oxcf.noise_sensitivity > 0) {
  vp9_denoiser_reset_on_first_frame(cpi);
}
#endif
```

Good:

```cpp
// Direct SVC layer state access guarded by use_svc
cpi->last_frame_dropped = 1;
if (cpi->use_svc) {
  cpi->svc.last_layer_dropped[cpi->svc.spatial_layer_id] = 1;
}

// Guard SVC helper call with cpi->use_svc check
if (cpi->use_svc) {
  denoise_svc_pickmode = vp9_denoise_svc_non_key(cpi);
}
if (cpi->denoiser.denoising_level > kDenLowLow && denoise_svc_pickmode) {
  denoise_svc(&cpi->denoiser, &cpi->svc, ...);
}

// Calling SVC-dependent reset helper guarded by use_svc
#if CONFIG_VP9_TEMPORAL_DENOISING
if (cpi->oxcf.noise_sensitivity > 0 && cpi->use_svc) {
  vp9_denoiser_reset_on_first_frame(cpi);
}
#endif
```
