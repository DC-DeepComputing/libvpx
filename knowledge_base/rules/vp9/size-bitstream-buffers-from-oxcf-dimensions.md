---
name: libvpx/vp9/size-bitstream-buffers-from-oxcf-dimensions
description: When allocating bitstream or worker buffers in the VP9 encoder, calculate sizes from configured dimensions (cpi->oxcf.width, cpi->oxcf.height) and bit depth rather than dynamic source pointers
rubric: correctness
severity: should_fix
---

**Rule.** In VP9 encoder buffer allocations (such as bitstream worker and tile
data buffers), compute allocation sizes using configured frame dimensions
(cpi->oxcf.width, cpi->oxcf.height) and bit depth (cpi->image_bps) rather than
reading dimensions from dynamic source buffer pointers (cpi->Source,
cpi->scaled_source, cpi->un_scaled_source). Ensure buffer sizing remains
invariant across frame-to-frame input buffer replacements and dynamic resolution
scaling.

**Why.** The VP9_COMP struct maintains multiple transient source pointers that
can be reassigned across frames, dynamically scaled, or uninitialized during
early setup. Sizing worker buffers based on configured cpi->oxcf dimensions and
cpi->image_bps guarantees that allocated bitstream buffers are sufficient for
the configured resolution and pixel format without risking buffer overflows or
stale pointer dereferences.

## Examples

Bad:

```cpp
static size_t encode_tiles_buffer_alloc_size(const VP9_COMP *cpi) {
  // Anti-pattern: Inspecting transient Source buffer dimensions
  return (size_t)(cpi->Source->y_crop_width * cpi->Source->y_crop_height +
                  2 * cpi->Source->uv_crop_width * cpi->Source->uv_crop_height);
}
```

Good:

```cpp
static size_t encode_tiles_buffer_alloc_size(const VP9_COMP *cpi) {
  const int image_bps = cpi->image_bps ? cpi->image_bps : 8;
  const int64_t alloc_size =
      (int64_t)cpi->oxcf.width * cpi->oxcf.height * image_bps / 8;
  return (size_t)alloc_size;
}
```
