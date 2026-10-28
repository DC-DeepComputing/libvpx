---
name: libvpx/validate-encoder-pts-only-when-image-non-null
description: When validating or tracking frame timestamps (PTS) and durations in encoder interfaces, guard checks with img != NULL to avoid failing flush calls
rubric: correctness
severity: should_fix
---

**Rule.** In encoder interface implementations (*_cx_iface.c), validate and
track frame presentation timestamps (pts), duration, and timestamp offsets only
when an input image is provided (img != NULL). Do not perform PTS range checks,
monotonicity validation, or PTS offset initialization during encoder flush calls
(img == NULL).

**Why.** When flushing an encoder at end-of-stream, vpx_codec_encode() is called
with img == NULL and arbitrary or zero PTS and duration values. Unconditionally
validating PTS or initializing pts_offset during flush calls causes false
VPX_CODEC_INVALID_PARAM errors or corrupts timestamp accounting.

## Examples

Bad:

```cpp
// In encoder_encode / vp8e_encode:
if (!ctx->pts_offset_initialized) {
  ctx->pts_offset = pts;
  ctx->pts_offset_initialized = 1;
}
if (pts < ctx->pts_offset) {
  vpx_internal_error(&cpi->common.error, VPX_CODEC_INVALID_PARAM,
                     "pts is smaller than initial pts");
}
if (img != NULL) {
  // process input frame
}
```

Good:

```cpp
// In encoder_encode / vp8e_encode:
if (img != NULL) {
  if (!ctx->pts_offset_initialized) {
    ctx->pts_offset = pts;
    ctx->pts_offset_initialized = 1;
  }
  if (pts < ctx->pts_offset) {
    vpx_internal_error(&cpi->common.error, VPX_CODEC_INVALID_PARAM,
                       "pts is smaller than initial pts");
  }
  // process input frame
}
```
