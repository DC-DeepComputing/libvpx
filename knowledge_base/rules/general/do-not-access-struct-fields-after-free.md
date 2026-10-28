---
name: libvpx/do-not-access-struct-fields-after-free
description: When invoking deallocation or cleanup functions on structures or buffer containers, do not access or modify constituent fields through local pointers after freeing
rubric: safety
severity: blocking
---

**Rule.** Do not access, dereference, or modify struct fields through local
pointers after calling a cleanup or deallocation function that frees the parent
structure, member arrays, or underlying buffer. When an error occurs and a
cleanup routine is invoked to free the container, immediately return or jump to
error handling without performing additional field updates on the deallocated
objects.

**Why.** Invoking a deallocation or cleanup function releases the parent
container and its constituent memory buffers back to the allocator. Any
subsequent read, write, or field reset through local pointers to those elements
constitutes a use-after-free bug that causes heap corruption and undefined
behavior.

## Examples

Bad:

```cpp
if (frame_stats->block_stats_list == NULL) {
  vpx_free_tpl_gop_stats(tpl_gop_stats);
  frame_stats->num_blocks = 0;  // Use-after-free
  return VPX_CODEC_MEM_ERROR;
}
```

Good:

```cpp
if (frame_stats->block_stats_list == NULL) {
  vpx_free_tpl_gop_stats(tpl_gop_stats);
  return VPX_CODEC_MEM_ERROR;
}
```
