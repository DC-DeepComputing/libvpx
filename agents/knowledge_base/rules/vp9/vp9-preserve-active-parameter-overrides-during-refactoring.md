---
name: libvpx/vp9/vp9-preserve-active-parameter-overrides-during-refactoring
description: When refactoring encoder algorithms or cleaning up tuning variables, do not inadvertently delete conditional or resolution-dependent overrides for parameters still in use
rubric: correctness
severity: should_fix
---

**Rule.** When refactoring encoder algorithms, redesigning heuristics, or
removing obsolete tuning variables, do not inadvertently delete conditional
(such as resolution-, frame-rate-, or flag-dependent) overrides for parameters
that continue to be used downstream. Check that all tuning parameters that
remain active across the algorithm retain their necessary operating-mode and
resolution adjustments.

**Why.** When obsolete threshold variables are stripped during an algorithm
redesign, removing adjacent conditional blocks can accidentally revert active
shared parameters (such as motion vector or denoising thresholds) to
uncalibrated defaults. This introduces silent behavioral discrepancies and
compression or visual quality regressions across different frame resolutions or
operating modes.

## Examples

Bad:

```cpp
// Refactoring noise estimation dropped resolution-dependent override for active threshold
#if CONFIG_VP9_TEMPORAL_DENOISING
  if (cpi->oxcf.noise_sensitivity > 0 && noise_est_svc(cpi)) {
    last_source = &cpi->denoiser.last_source;
  }
#endif
```

Good:

```cpp
// Preserved resolution-dependent override for active threshold thresh_consec_zeromv
#if CONFIG_VP9_TEMPORAL_DENOISING
  if (cpi->oxcf.noise_sensitivity > 0 && noise_est_svc(cpi)) {
    last_source = &cpi->denoiser.last_source;
    // Tune threshold for different resolutions when denoising is enabled.
    if (cm->width > 640 && cm->width <= 1920) {
      thresh_consec_zeromv = 2;
    }
  }
#endif
```
