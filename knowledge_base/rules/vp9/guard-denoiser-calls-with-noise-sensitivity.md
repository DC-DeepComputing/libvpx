---
name: libvpx/vp9/guard-denoiser-calls-with-noise-sensitivity
description: When invoking VP9 temporal denoiser or noise estimation routines under CONFIG_VP9_TEMPORAL_DENOISING, guard execution with a check that noise_sensitivity is greater than zero
rubric: correctness
severity: should_fix
---

**Rule.** When invoking VP9 temporal denoiser or noise estimation routines under
#if CONFIG_VP9_TEMPORAL_DENOISING, guard execution with a check that noise
sensitivity is enabled (cpi->oxcf.noise_sensitivity > 0, along with SVC active
checks such as cpi->use_svc or denoise_svc(cpi) when applicable). Do not execute
denoiser reset, update, or filtering functions solely under compile-time #if
guards when runtime denoising is disabled.

**Why.** In VP9, CONFIG_VP9_TEMPORAL_DENOISING is a compile-time build
configuration flag, but denoising is dynamically activated via
cpi->oxcf.noise_sensitivity > 0. Calling denoiser helpers unconditionally when
noise_sensitivity == 0 causes unnecessary runtime overhead and unintended state
modifications (such as reference buffer resets) when denoising is disabled.

## Examples

Bad:

```cpp
#if CONFIG_VP9_TEMPORAL_DENOISING
  // Bad: executes unconditionally even when noise_sensitivity is 0 (disabled).
  vp9_denoiser_reset_on_first_frame(cpi);
#endif
```

Good:

```cpp
#if CONFIG_VP9_TEMPORAL_DENOISING
  // Good: gates denoiser reset on active noise sensitivity and SVC state.
  if (cpi->oxcf.noise_sensitivity > 0 && cpi->use_svc) {
    vp9_denoiser_reset_on_first_frame(cpi);
  }
#endif
```
