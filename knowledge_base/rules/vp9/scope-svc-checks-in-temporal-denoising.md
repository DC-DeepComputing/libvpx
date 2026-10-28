---
name: libvpx/vp9/scope-svc-checks-in-temporal-denoising
description: When implementing VP9 temporal denoising logic, scope cpi->use_svc checks to SVC-specific calculations rather than gating shared denoiser operations or stats resets
rubric: correctness
severity: should_fix
---

**Rule.** In VP9 temporal denoising (CONFIG_VP9_TEMPORAL_DENOISING), scope
cpi->use_svc conditions strictly to SVC-specific calculations (such as
vp9_denoise_svc_non_key(cpi)) instead of combining cpi->use_svc into the outer
cpi->oxcf.noise_sensitivity > 0 check. Ensure shared denoiser operations,
including vp9_denoiser_reset_frame_stats(), execute for all active denoiser
streams regardless of whether SVC is enabled.

**Why.** Temporal denoising is supported for both single-layer non-SVC and
multi-layer SVC encodings. Gating shared denoiser logic or the outer noise
sensitivity check on cpi->use_svc inadvertently disables frame statistics resets
and denoiser routines for all non-SVC streams.

## Examples

Bad:

```cpp
#if CONFIG_VP9_TEMPORAL_DENOISING
  if (cpi->oxcf.noise_sensitivity > 0 && cpi->use_svc) {
    denoise_svc_pickmode = vp9_denoise_svc_non_key(cpi);
    if (cpi->denoiser.denoising_level > kDenLowLow && denoise_svc_pickmode)
      vp9_denoiser_reset_frame_stats(ctx);
  }
#endif
```

Good:

```cpp
#if CONFIG_VP9_TEMPORAL_DENOISING
  if (cpi->oxcf.noise_sensitivity > 0) {
    if (cpi->use_svc) denoise_svc_pickmode = vp9_denoise_svc_non_key(cpi);
    if (cpi->denoiser.denoising_level > kDenLowLow && denoise_svc_pickmode)
      vp9_denoiser_reset_frame_stats(ctx);
  }
#endif
```
