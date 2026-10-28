---
name: libvpx/validate-input-image-inside-validate-img
description: In encoder codec interface implementations (*_cx_iface.c), perform input image property and pixel validation inside validate_img() rather than inline in encoder_encode().
rubric: design
severity: nit
---

**Rule.** In encoder codec interface implementations (*_cx_iface.c), perform
input image validations (such as pixel value bounds, format, dimension, or
stride checks) inside the dedicated validate_img() helper function rather than
inline in encoder_encode(). Ensure encoder_encode() delegates all vpx_image_t
validation directly to validate_img().

**Why.** validate_img() is the designated helper in codec interface files for
validating incoming vpx_image_t structures before encoding begins. Inlining
image property and sample checks into encoder_encode() bloats the top-level
encode dispatch function and scatters image validation logic across multiple
places.

## Examples

Bad:

```cpp
// In encoder_encode():
if (img != NULL) {
  res = validate_img(ctx, img);
  if (res != VPX_CODEC_OK) return res;

#if CONFIG_VP9_HIGHBITDEPTH
  if (ctx->extra_cfg.validate_input_hbd &&
      (img->fmt & VPX_IMG_FMT_HIGHBITDEPTH) &&
      ctx->oxcf.input_bit_depth > 8) {
    res = validate_input_pixels(ctx, img);
    if (res != VPX_CODEC_OK) return res;
  }
#endif
}
```

Good:

```cpp
// In validate_img():
static vpx_codec_err_t validate_img(vpx_codec_alg_priv_t *ctx,
                                    const vpx_image_t *img) {
  ...
#if CONFIG_VP9_HIGHBITDEPTH
  if (ctx->extra_cfg.validate_input_hbd &&
      (img->fmt & VPX_IMG_FMT_HIGHBITDEPTH) &&
      ctx->oxcf.input_bit_depth > 8) {
    res = validate_input_pixels(ctx, img);
    if (res != VPX_CODEC_OK) return res;
  }
#endif
  return VPX_CODEC_OK;
}
```
