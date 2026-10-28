---
name: libvpx/vp8/omit-redundant-buffer-alloc-field-zeroing
description: When implementing or updating buffer allocation routines for context structs like VP8_COMMON or VP9_COMMON, omit redundant field zeroing already handled during struct creation or deallocation.
rubric: readability
severity: nit
---

**Rule.** Do not redundantly zero-initialize or reset struct fields in buffer
allocation routines (such as vp8_alloc_frame_buffers or
vp9_alloc_context_buffers) if the parent context struct is already
zero-initialized on creation and properly reset in corresponding deallocation
functions. Rely on the deallocation and creation functions to manage field
cleanup and default states.

**Why.** Context structs are zero-initialized upon allocation, and buffer
allocation routines typically invoke deallocation routines at their entry point
to release and reset existing buffers. Manually re-zeroing tracking fields or
flags in allocation functions creates redundant code clutter and risks
inconsistent state management if deallocation logic is updated.

## Examples

Bad:

```cpp
int vp8_alloc_frame_buffers(VP8_COMMON *oci, int width, int height) {
  vp8_de_alloc_frame_buffers(oci);

  // Redundant: oci was zeroed on creation and reset in vp8_de_alloc_frame_buffers
  oci->post_proc_buffer_int_used = 0;

  if (vp8_yv12_alloc_frame_buffer(&oci->post_proc_buffer, width, height,
                                  VP8BORDERINPIXELS) < 0) {
    return -1;
  }
  oci->post_proc_buffer_int_used = 1;
  return 0;
}
```

Good:

```cpp
int vp8_alloc_frame_buffers(VP8_COMMON *oci, int width, int height) {
  vp8_de_alloc_frame_buffers(oci);

  if (vp8_yv12_alloc_frame_buffer(&oci->post_proc_buffer, width, height,
                                  VP8BORDERINPIXELS) < 0) {
    return -1;
  }
  oci->post_proc_buffer_int_used = 1;
  return 0;
}
```
