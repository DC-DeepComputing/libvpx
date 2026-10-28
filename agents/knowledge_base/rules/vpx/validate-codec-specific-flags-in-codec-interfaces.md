---
name: libvpx/vpx/validate-codec-specific-flags-in-codec-interfaces
description: When validating codec-specific frame flags or configuration prerequisites, validate within codec interface implementations rather than generic dispatch functions like vpx_codec_encode
rubric: design
severity: should_fix
---

**Rule.** Validate codec-specific frame flags (such as
`VPX_EFLAG_CALCULATE_PSNR`) and codec configuration constraints within each
codec's interface implementation (e.g., in `vp8_cx_iface.c` or `vp9_cx_iface.c`)
rather than in top-level generic dispatch functions like `vpx_codec_encode()`.
Use `vpx_internal_error()` within the codec interface to report invalid flag
combinations or unsupported modes.

**Why.** `vpx_codec_encode()` in `vpx/src/vpx_encoder.c` is a generic,
codec-agnostic dispatcher responsible only for top-level argument and capability
validation. Placing codec-specific flag checks in codec interface
implementations ensures proper error reporting via `vpx_internal_error()` on the
codec error context and maintains clean encapsulation between generic API
wrappers and codec implementations.

## Examples

Bad:

```cpp
// In vpx/src/vpx_encoder.c (vpx_codec_encode):
else if ((flags & VPX_EFLAG_CALCULATE_PSNR) &&
         (!(ctx->iface->caps & VPX_CODEC_CAP_PSNR) ||
          ctx->config.enc->g_lag_in_frames != 0)) {
  ctx->err_detail = "Conflicting PSNR flags.";
  res = VPX_CODEC_INCAPABLE;
}
```

Good:

```cpp
// In vp8/vp8_cx_iface.c and vp9/vp9_cx_iface.c (encoder encode handler):
if ((flags & VPX_EFLAG_CALCULATE_PSNR) && ctx->cfg.g_lag_in_frames != 0) {
  vpx_internal_error(
      &ctx->cpi->common.error, VPX_CODEC_INCAPABLE,
      "Cannot calculate per-frame PSNR when g_lag_in_frames is nonzero");
}
```
