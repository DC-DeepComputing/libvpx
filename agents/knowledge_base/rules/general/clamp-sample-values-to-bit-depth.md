---
name: libvpx/clamp-sample-values-to-bit-depth
description: When populating or validating VPX_IMG_FMT_HIGHBITDEPTH image buffers for vpx_codec_encode, clamp or validate samples within [0..(1 << bit_depth) - 1] without conditioning checks on bit_depth > 8.
rubric: safety
severity: should_fix
---

**Rule.** When populating or validating vpx_image_t buffers with
VPX_IMG_FMT_HIGHBITDEPTH for vpx_codec_encode() or codec interface validation,
ensure all 16-bit sample values are clamped or verified within [0..(1 <<
bit_depth) - 1]. Do not condition sample validation or clamping on bit_depth >
8; 8-bit samples stored in 16-bit unsigned short containers can also exceed 255
and must be bounded.

**Why.** In libvpx, VPX_IMG_FMT_HIGHBITDEPTH indicates that pixel samples are
stored in 16-bit words (unsigned short / uint16_t), which can hold values up to
65535 regardless of whether bit_depth is 8, 10, or 12. If sample values exceed
(1 << bit_depth) - 1, out-of-range inputs bypass encoder constraints, causing
undefined behavior, integer overflow, or memory corruption during quantization
and transform operations.

## Examples

Bad:

```cpp
// Bad (validating): Skipping sample validation when bit_depth == 8
#if CONFIG_VP9_HIGHBITDEPTH
if (ctx->extra_cfg.validate_hbd_input &&
    (img->fmt & VPX_IMG_FMT_HIGHBITDEPTH) &&
    ctx->oxcf.input_bit_depth > 8) {
  // Bypasses validation for 8-bit samples stored in 16-bit unsigned short
}
#endif

// Bad (populating): Copying unvalidated 16-bit data without clamping
const size_t bytespp = (img->fmt & VPX_IMG_FMT_HIGHBITDEPTH) ? 2 : 1;
memcpy(buf, data, nb);
vpx_codec_encode(codec, img, pts, 1, flags, quality);
```

Good:

```cpp
// Good (validating): Validating all 16-bit sample buffers regardless of bit_depth
#if CONFIG_VP9_HIGHBITDEPTH
if (ctx->extra_cfg.validate_hbd_input &&
    (img->fmt & VPX_IMG_FMT_HIGHBITDEPTH)) {
  // Validates pixel bounds: handles bit_depth == 8 stored in unsigned short
}
#endif

// Good (populating): Clamping samples to [0..(1 << img->bit_depth) - 1]
const size_t bytespp = (img->fmt & VPX_IMG_FMT_HIGHBITDEPTH) ? 2 : 1;
clamp_samples(buf, nb, (1 << img->bit_depth) - 1);
vpx_codec_encode(codec, img, pts, 1, flags, quality);
```
