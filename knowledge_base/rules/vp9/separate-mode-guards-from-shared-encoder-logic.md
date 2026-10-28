---
name: libvpx/vp9/separate-mode-guards-from-shared-encoder-logic
description: When conditioning shared encoder logic on mode-specific flags like cpi->use_svc, evaluate mode-specific branches separately to prevent bypassing shared processing in standard modes
rubric: correctness
severity: should_fix
---

**Rule.** Do not combine mode-specific guards (such as cpi->use_svc or special
operating mode flags) into outer conditional expressions that also enclose
shared operations or state resets. Evaluate mode-specific branches in their own
inner conditionals or separate statements so that standard non-mode encoding
paths continue to execute all shared logic.

**Why.** Gating shared encoder logic (such as denoiser frame stats resetting or
rate control updates) behind a mode-specific flag inadvertently skips that logic
for standard encodings, causing behavioral discrepancies and breaking
bitexactness.

## Examples

Bad:

```cpp
if (cpi->oxcf.noise_sensitivity > 0 && cpi->use_svc) {
  denoise_svc_pickmode = vp9_denoise_svc_non_key(cpi);
  if (cpi->denoiser.denoising_level > kDenLowLow && denoise_svc_pickmode)
    vp9_denoiser_reset_frame_stats(ctx);
}
```

Good:

```cpp
if (cpi->oxcf.noise_sensitivity > 0) {
  if (cpi->use_svc) denoise_svc_pickmode = vp9_denoise_svc_non_key(cpi);
  if (cpi->denoiser.denoising_level > kDenLowLow && denoise_svc_pickmode)
    vp9_denoiser_reset_frame_stats(ctx);
}
```
