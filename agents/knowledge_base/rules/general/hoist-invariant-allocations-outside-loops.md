---
name: libvpx/hoist-invariant-allocations-outside-loops
description: When allocating dynamic memory buffers inside loops where buffer dimensions are invariant across iterations, hoist the allocation before the loop and reuse it
rubric: safety
severity: should_fix
---

**Rule.** Do not allocate and free dynamic memory buffers inside loops when
buffer dimensions are invariant across iterations. Hoist the allocation before
the loop, reuse the buffer across iterations (reinitializing with memset or
zeroing macros if needed), and free it after the loop.

**Why.** Repeatedly calling dynamic memory allocators and deallocators inside
loops introduces significant heap management overhead and memory fragmentation.
Hoisting invariant allocations eliminates heap churn while reusing the same
allocated block across iterations.

## Examples

Bad:

```cpp
for (idx = gf_index; idx <= gop_length; ++idx) {
  CHECK_MEM_ERROR(&cm->error, encode_frame_decision.sb_params_list,
                  vpx_calloc(frame_height_sb * frame_width_sb,
                             sizeof(*encode_frame_decision.sb_params_list)));
  // Process frame using sb_params_list...
  vpx_free(encode_frame_decision.sb_params_list);
}
```

Good:

```cpp
CHECK_MEM_ERROR(
    &cm->error, encode_frame_decision.sb_params_list,
    vpx_malloc(frame_height_sb * frame_width_sb *
               sizeof(*encode_frame_decision.sb_params_list)));
for (idx = gf_index; idx <= gop_length; ++idx) {
  memset(encode_frame_decision.sb_params_list, 0,
         frame_height_sb * frame_width_sb *
             sizeof(*encode_frame_decision.sb_params_list));
  // Process frame using sb_params_list...
}
vpx_free(encode_frame_decision.sb_params_list);
```
