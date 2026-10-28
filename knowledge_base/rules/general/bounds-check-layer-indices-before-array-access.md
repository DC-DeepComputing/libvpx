---
name: libvpx/bounds-check-layer-indices-before-array-access
description: When indexing fixed-capacity arrays or buffers using dynamic layer indices, verify or assert the index is within array bounds before access.
rubric: safety
severity: should_fix
---

**Rule.** When indexing fixed-size buffers, debug file handles, or configuration
tables using dynamic indices such as spatial or temporal layer IDs, ensure the
index is explicitly bounds-checked against the array's capacity before access.
Do not assume dynamic layer indices are bounded by smaller fixed array
dimensions, especially when layer counts can vary up to maximum configuration
limits like VPX_SS_MAX_LAYERS.

**Why.** Dynamic indices like spatial or temporal layer IDs can exceed the
allocated size of fixed-capacity buffers or debug arrays when configurations use
higher layer counts. Unchecked array accesses result in out-of-bounds memory
access, corrupted state, or process crashes.

## Examples

Bad:

```cpp
#ifdef OUTPUT_YUV_SVC_SRC
  if (is_one_pass_cbr_svc(cpi)) {
    // Spatial layer ID can exceed the 3-element fixed array size.
    vpx_write_yuv_frame(yuv_svc_src[cpi->svc.spatial_layer_id], cpi->Source);
  }
#endif
```

Good:

```cpp
#ifdef OUTPUT_YUV_SVC_SRC
  // Ensure spatial layer ID is within the bounds of yuv_svc_src.
  if (is_one_pass_cbr_svc(cpi) && cpi->svc.spatial_layer_id < 3) {
    vpx_write_yuv_frame(yuv_svc_src[cpi->svc.spatial_layer_id], cpi->Source);
  }
#endif
```
