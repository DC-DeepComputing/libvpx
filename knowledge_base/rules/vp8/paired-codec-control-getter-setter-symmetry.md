---
name: libvpx/vp8/paired-codec-control-getter-setter-symmetry
description: When updating or extending paired codec control getter and setter handlers (ctrl_get_* and ctrl_set_*) and shared configuration structs, keep counterpart field handling symmetric.
rubric: correctness
severity: should_fix
---

**Rule.** When adding, updating, or extending paired codec control getters and
setters (such as ctrl_get_* and ctrl_set_* dispatch handlers in codec interface
files) and their associated configuration structs, keep both implementations
symmetric. Ensure that every configurable field read or written in one handler
has its counterpart updated or queried in the paired handler.

**Why.** Asymmetric getter and setter implementations cause silent configuration
desynchronization where callers cannot accurately query values they set or where
newly added struct fields are ignored when configuring encoder behavior.

## Examples

Bad:

```cpp
// In ctrl_get_svc_ref_frame_config, reference flags are read, but
// ctrl_set_svc_ref_frame_config fails to set them symmetrically:
static vpx_codec_err_t ctrl_set_svc_ref_frame_config(vpx_codec_alg_priv_t *ctx,
                                                     va_list args) {
  VP9_COMP *const cpi = ctx->cpi;
  vpx_svc_ref_frame_config_t *data =
      va_arg(args, vpx_svc_ref_frame_config_t *);
  for (int sl = 0; sl < cpi->svc.number_spatial_layers; ++sl) {
    cpi->svc.lst_fb_idx[sl] = data->lst_fb_idx[sl];
    cpi->svc.gld_fb_idx[sl] = data->gld_fb_idx[sl];
    cpi->svc.alt_fb_idx[sl] = data->alt_fb_idx[sl];
  }
  return VPX_CODEC_OK;
}
```

Good:

```cpp
// Symmetric setter updating all counterpart fields matching the getter:
static vpx_codec_err_t ctrl_set_svc_ref_frame_config(vpx_codec_alg_priv_t *ctx,
                                                     va_list args) {
  VP9_COMP *const cpi = ctx->cpi;
  vpx_svc_ref_frame_config_t *data =
      va_arg(args, vpx_svc_ref_frame_config_t *);
  for (int sl = 0; sl < cpi->svc.number_spatial_layers; ++sl) {
    cpi->svc.reference_last[sl] = data->reference_last[sl];
    cpi->svc.reference_golden[sl] = data->reference_golden[sl];
    cpi->svc.reference_altref[sl] = data->reference_alt_ref[sl];
    cpi->svc.lst_fb_idx[sl] = data->lst_fb_idx[sl];
    cpi->svc.gld_fb_idx[sl] = data->gld_fb_idx[sl];
    cpi->svc.alt_fb_idx[sl] = data->alt_fb_idx[sl];
  }
  return VPX_CODEC_OK;
}
```
