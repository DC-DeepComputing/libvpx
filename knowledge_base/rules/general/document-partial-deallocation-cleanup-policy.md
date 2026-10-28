---
name: libvpx/document-partial-deallocation-cleanup-policy
description: When defining or modifying partial deallocation or buffer reset functions, document which resources are freed and what struct fields are preserved
rubric: readability
severity: nit
---

**Rule.** When defining or updating deallocation, teardown, or reset functions
that partially clear a persistent struct or context without destroying or
zeroing the entire object, document the cleanup policy above the function
declaration or definition. Clarify which heap buffers and indirect references
are released and reset versus which configuration fields, dimensions, or state
remain preserved and valid across reallocations.

**Why.** In complex codec contexts, partial cleanup functions often release
internal frame or mode buffers during resolution changes or allocation failures
while keeping outer dimensions and configuration intact. Without an explicit
comment describing the scope of cleanup, maintainers cannot tell whether unreset
fields are intentionally preserved or accidentally leaked/dangling.

## Examples

Bad:

```cpp
// (no comment explaining what is released vs preserved)
void vp8_de_alloc_frame_buffers(VP8_COMMON *oci) {
  int i;
  for (i = 0; i < NUM_YV12_BUFFERS; ++i) {
    vp8_yv12_de_alloc_frame_buffer(&oci->yv12_fb[i]);
    oci->fb_idx_ref_cnt[i] = 0;
  }
  vpx_free(oci->above_context);
  oci->above_context = NULL;
}
```

Good:

```cpp
// Frees all frame and post-processing buffers and indirect mode/context
// allocations while preserving dimensions and codec configuration.
void vp8_de_alloc_frame_buffers(VP8_COMMON *oci) {
  int i;
  for (i = 0; i < NUM_YV12_BUFFERS; ++i) {
    vp8_yv12_de_alloc_frame_buffer(&oci->yv12_fb[i]);
    oci->fb_idx_ref_cnt[i] = 0;
  }
  vpx_free(oci->above_context);
  oci->above_context = NULL;
}
```
