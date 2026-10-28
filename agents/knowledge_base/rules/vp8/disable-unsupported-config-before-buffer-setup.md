---
name: libvpx/vp8/disable-unsupported-config-before-buffer-setup
description: When disabling unsupported encoder features or incompatible configuration flags, clear them before buffer allocation or frame setup routines rather than downstream in encoding loops
rubric: correctness
severity: should_fix
---

**Rule.** When disabling unsupported feature combinations or overriding invalid
configuration options (such as clearing noise_sensitivity for high-bitdepth
frames), update or disable the configuration flags before buffer allocation and
frame setup routines execute. Do not defer configuration overrides to downstream
frame-encoding loops where early buffer allocation would already have run using
stale configuration flags.

**Why.** Overriding feature flags late inside frame-encoding loops leaves
unsupported features enabled during initial frame-level buffer allocation,
causing unnecessary memory allocation or errors on unsupported bit depths and
formats.

## Examples

Bad:

```cpp
// In frame encoding loop after buffer initialization has already executed:
#if CONFIG_VP9_HIGHBITDEPTH
if (cm->bit_depth > 8) {
  cpi->oxcf.noise_sensitivity = 0;
}
#endif
```

Good:

```cpp
// In vp9_receive_raw_frame before buffer setup routines run:
#if CONFIG_VP9_HIGHBITDEPTH
if (cpi->common.bit_depth > 8) {
  cpi->oxcf.noise_sensitivity = 0;
}
#endif

#if CONFIG_VP9_TEMPORAL_DENOISING
setup_denoiser_buffer(cpi);
#endif
```
