---
name: libvpx/vp8/update-frame-buffer-geometry-fields-together
description: When overriding or caching frame buffer descriptors (such as switching to unscaled source buffers), update all related geometry attributes—plane pointers, stride, width, and height—together.
rubric: correctness
severity: should_fix
---

**Rule.** When overriding, unpacking, or caching frame buffer descriptors
(YV12_BUFFER_CONFIG)—such as switching between Source, un_scaled_source, or
scaled_source buffers—ensure all related buffer pointer and geometry fields
(y_buffer, y_stride, y_width, y_height, and UV equivalents) are updated
together. Do not update plane pointers and width while leaving height or stride
initialized from a different buffer.

**Why.** Incomplete buffer geometry updates leave mismatched dimensions or
strides between plane pointers and frame dimensions, causing corrupted motion
search, incorrect boundary checks, out-of-bounds memory access, or video
distortion downstream.

## Examples

Bad:

```cpp
if (cpi->use_svc) {
  src_y = cpi->un_scaled_source->y_buffer;
  src_ystride = cpi->un_scaled_source->y_stride;
  src_width = cpi->un_scaled_source->y_width;
  // Bug: src_height remains initialized from cpi->Source->y_height.
}
```

Good:

```cpp
if (cpi->use_svc) {
  src_y = cpi->un_scaled_source->y_buffer;
  src_ystride = cpi->un_scaled_source->y_stride;
  src_width = cpi->un_scaled_source->y_width;
  src_height = cpi->un_scaled_source->y_height;
}
```
