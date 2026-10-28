---
name: libvpx/test/initialize-allocated-test-image-planes
description: When allocating test image buffers with vpx_img_alloc before encoding, initialize all image planes (Y, U, V) to avoid MemorySanitizer failures
rubric: correctness
severity: should_fix
---

**Rule.** When allocating test image frames (such as via `vpx_img_alloc()`) to
pass into `vpx_codec_encode()`, initialize all image plane buffers (including
chroma planes `VPX_PLANE_U` and `VPX_PLANE_V`) before encoding. Populate pixel
data or memset plane memory across their respective widths and heights
(accounting for subsampling and row stride) rather than leaving allocated image
memory uninitialized.

**Why.** `vpx_img_alloc()` allocates heap memory without zeroing or initializing
pixel buffers. Passing uninitialized image buffers into the encoder leads to
uninitialized memory reads during frame processing and transforms, causing
MemorySanitizer (MSan) test failures in CI test runs.

## Examples

Bad:

```cpp
vpx_image_t *img = vpx_img_alloc(nullptr, img_fmt, cfg.g_w, cfg.g_h, 1);
// Uninitialized plane buffers cause MemorySanitizer (MSan) test failures.
ASSERT_EQ(vpx_codec_encode(&ctx, img, pts, duration, 0, deadline),
          VPX_CODEC_OK);
```

Good:

```cpp
vpx_image_t *img = vpx_img_alloc(nullptr, img_fmt, cfg.g_w, cfg.g_h, 1);
for (unsigned int y = 0; y < img->d_h; y++) {
  for (unsigned int x = 0; x < img->d_w; x++) {
    img->planes[0][y * img->stride[0] + x] = ((x ^ y) * 127) & 0xFF;
  }
}
const unsigned int uv_height = (img->d_h + 1) >> 1;
for (int plane : { VPX_PLANE_U, VPX_PLANE_V }) {
  memset(img->planes[plane], 0, img->stride[plane] * uv_height);
}
ASSERT_EQ(vpx_codec_encode(&ctx, img, pts, duration, 0, deadline),
          VPX_CODEC_OK);
```
