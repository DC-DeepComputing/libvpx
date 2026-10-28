---
name: libvpx/vp8/sync-shared-codec-controls-across-vp8-and-vp9
description: When modifying parameter validation, clamping, or dispatch for shared VP8/VP9 codec controls, update both vp8 and vp9 interface implementations consistently.
rubric: correctness
severity: should_fix
---

**Rule.** When adding, updating, or modifying parameter validation, value
clamping, or dispatch logic for shared codec controls (such as VP8E_SET_*,
VP8E_GET_*, or VP8D_* defined in vpx/vp8cx.h or vpx/vp8dx.h), verify and update
both VP8 and VP9 interface implementations (vp8_cx_iface.c and vp9_cx_iface.c,
or vp8_dx_iface.c and vp9_dx_iface.c) consistently. Do not modify the valid
parameter range or clamping behavior in only one codec when the control is
shared across both.

**Why.** Shared codec controls defined in public headers are dispatched by both
VP8 and VP9 encoder and decoder interfaces. Inconsistent validation, clamping,
or error handling between the two implementations causes diverging behavior for
identical control IDs and can lead to unexpected out-of-bounds parameter values
or rejected valid configurations.

## Examples

Bad:

```cpp
// vp9/vp9_cx_iface.c: Clamping cpu_used parameter without updating vp8_cx_iface.c
static vpx_codec_err_t ctrl_set_cpuused(vpx_codec_alg_priv_t *ctx,
                                         va_list args) {
  struct vp9_extracfg extra_cfg = ctx->extra_cfg;
  extra_cfg.cpu_used = CAST(VP8E_SET_CPUUSED, args);
  extra_cfg.cpu_used = clamp(extra_cfg.cpu_used, -9, 9);
  return set_encoder_config(ctx, &extra_cfg);
}
// vp8/vp8_cx_iface.c is left unchanged with disparate validation/clamping
```

Good:

```cpp
// vp9/vp9_cx_iface.c: Update VP8E_SET_CPUUSED handling in VP9
static vpx_codec_err_t ctrl_set_cpuused(vpx_codec_alg_priv_t *ctx,
                                         va_list args) {
  struct vp9_extracfg extra_cfg = ctx->extra_cfg;
  extra_cfg.cpu_used = CAST(VP8E_SET_CPUUSED, args);
  extra_cfg.cpu_used = clamp(extra_cfg.cpu_used, -9, 9);
  return set_encoder_config(ctx, &extra_cfg);
}

// vp8/vp8_cx_iface.c: Also update shared VP8E_SET_CPUUSED handling in VP8
static vpx_codec_err_t set_cpu_used(vpx_codec_alg_priv_t *ctx, va_list args) {
  struct vp8_extracfg extra_cfg = ctx->extra_cfg;
  extra_cfg.cpu_used = CAST(VP8E_SET_CPUUSED, args);
  extra_cfg.cpu_used = clamp(extra_cfg.cpu_used, -16, 16);
  return set_encoder_config(ctx, &extra_cfg);
}
```
