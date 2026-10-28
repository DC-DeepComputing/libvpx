---
name: libvpx/validate-codec-control-parameters
description: In codec interface files, validate incoming control and config parameters before state mutation using RANGE_CHECK macros instead of ternary coercion, deferring cross-parameter checks to frame ingestion
rubric: correctness
severity: should_fix
---

**Rule.** When implementing or modifying codec control handlers (ctrl_*) or
configuration functions in codec interface files (*_iface.c), validate all
incoming arguments, user-provided configuration values, and extracted struct
fields (checking bounds, non-negative ranges, and non-null pointers) before
mutating internal context state, returning VPX_CODEC_INVALID_PARAM on invalid
inputs. Use standard RANGE_CHECK macros (such as RANGE_CHECK, RANGE_CHECK_HI,
RANGE_CHECK_LO, or RANGE_CHECK_BOOL) for range validation instead of silently
coercing values with boolean ternary expressions (e.g., `val == 1 ? 1 : 0`).
Ensure validation checks evaluate the incoming argument or unpacked struct
fields (e.g., data->field or params->field) rather than internal state fields
(e.g., svc->field or cpi->field), which may be uninitialized or stale. Do not
perform cross-parameter or frame-dependent validation (such as color space
compatibility against codec profile and chroma subsampling) inside ctrl_*
handlers; validate interdependent configuration state during frame ingestion
(e.g., in vp9_receive_raw_frame()) since configuration controls can be invoked
in arbitrary order.

**Why.** Mutating internal codec context fields before or during input
validation leaves state partially corrupted when a check fails. Silently
coercing configuration settings with ternary expressions masks invalid inputs
from callers and restricts future extensibility for multi-value modes, whereas
RANGE_CHECK macros explicitly enforce valid ranges and return
VPX_CODEC_INVALID_PARAM. Checking internal state struct fields instead of
incoming argument structs evaluates stale or uninitialized data and bypasses
bounds checks. Furthermore, codec controls and configuration updates can occur
in arbitrary order, and frame properties like chroma subsampling belong to input
frames, so cross-parameter and frame-coupled constraints must be verified at
frame ingestion rather than in individual control setters.

## Examples

Bad:

```cpp
// Example 1: Missing parameter validation or checking internal state instead of incoming struct.
static vpx_codec_err_t ctrl_set_svc_params(vpx_codec_alg_priv_t *ctx,
                                           va_list args) {
  struct vp9_extracfg *const extra_cfg = &ctx->extra_cfg;
  const vpx_svc_extra_cfg_t *const params =
      va_arg(args, const vpx_svc_extra_cfg_t *);
  extra_cfg->scaling_factor_num[sl] = params->scaling_factor_num[sl];
  // Bad: Checking internal state field svc->spatial_layer_id or checking after mutation.
  if (svc->spatial_layer_id < 0 ||
      svc->spatial_layer_id >= (int)ctx->cfg.ss_number_layers) {
    return VPX_CODEC_INVALID_PARAM;
  }
  return VPX_CODEC_OK;
}

// Example 2: Silently coercing configuration input with ternary expression.
ctx->pbi->row_mt = (ctx->row_mt == 1) ? 1 : 0;

// Example 3: Checking interdependent profile/subsampling in control setter.
static vpx_codec_err_t ctrl_set_color_space(vpx_codec_alg_priv_t *ctx,
                                            va_list args) {
  struct vp9_extracfg extra_cfg = ctx->extra_cfg;
  extra_cfg.color_space = CAST(VP9E_SET_COLOR_SPACE, args);
  if (extra_cfg.color_space == VPX_CS_SRGB &&
      ctx->cpi->common.profile != PROFILE_1) {
    return VPX_CODEC_INVALID_PARAM;
  }
  return update_extra_cfg(ctx, &extra_cfg);
}
```

Good:

```cpp
// Example 1: Validate incoming struct argument and bounds before state mutation.
static vpx_codec_err_t ctrl_set_svc_params(vpx_codec_alg_priv_t *ctx,
                                           va_list args) {
  struct vp9_extracfg *const extra_cfg = &ctx->extra_cfg;
  const vpx_svc_extra_cfg_t *const params =
      va_arg(args, const vpx_svc_extra_cfg_t *);
  if (params == NULL || params->spatial_layer_id < 0 ||
      params->spatial_layer_id >= (int)ctx->cfg.ss_number_layers) {
    return VPX_CODEC_INVALID_PARAM;
  }
  extra_cfg->scaling_factor_num[sl] = params->scaling_factor_num[sl];
  return VPX_CODEC_OK;
}

// Example 2: Validate input range with RANGE_CHECK macro returning error on invalid value.
RANGE_CHECK(ctx, row_mt, 0, 1);
ctx->pbi->row_mt = ctx->row_mt;

// Example 3: Set parameter in control setter, validate dependencies at frame ingest.
static vpx_codec_err_t ctrl_set_color_space(vpx_codec_alg_priv_t *ctx,
                                            va_list args) {
  struct vp9_extracfg extra_cfg = ctx->extra_cfg;
  extra_cfg.color_space = CAST(VP9E_SET_COLOR_SPACE, args);
  return update_extra_cfg(ctx, &extra_cfg);
}

// In vp9_receive_raw_frame():
if (cm->color_space == VPX_CS_SRGB &&
    (cm->profile == PROFILE_0 || cm->profile == PROFILE_2)) {
  vpx_internal_error(&cm->error, VPX_CODEC_INVALID_PARAM,
                     "SRGB color space requires profile 1 or 3");
}
```
