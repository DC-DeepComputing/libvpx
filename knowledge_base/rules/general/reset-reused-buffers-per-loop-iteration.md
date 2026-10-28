---
name: libvpx/reset-reused-buffers-per-loop-iteration
description: When reusing allocated buffers across loop iterations for operations that conditionally populate them, reset memory with memset each iteration rather than relying on one-time zero-initialization
rubric: correctness
severity: should_fix
---

**Rule.** When allocating a buffer or struct once outside a loop and reusing it
across iterations for an operation or callback that only conditionally populates
it, reset or re-initialize the buffer (e.g., using memset() or vp9_zero())
inside the loop at the start of each iteration. Do not rely solely on one-time
zero-allocation (such as vpx_calloc()) outside the loop.

**Why.** If memory is only zero-initialized once prior to entering the loop, any
iteration where the callee leaves fields unpopulated or untouched will retain
stale data from earlier iterations rather than receiving clean default values.

## Examples

Bad:

```cpp
CHECK_MEM_ERROR(&cm->error, decision.sb_params_list,
                vpx_calloc(count, sizeof(*decision.sb_params_list)));
for (idx = gf_index; idx <= gop_length; ++idx) {
  // If get_encodeframe_decision only sets sb_params_list on some frames,
  // subsequent frames retain stale data from earlier iterations.
  codec_status = vp9_extrc_get_encodeframe_decision(ext_ratectrl, idx, &decision);
}
```

Good:

```cpp
CHECK_MEM_ERROR(&cm->error, decision.sb_params_list,
                vpx_malloc(count * sizeof(*decision.sb_params_list)));
for (idx = gf_index; idx <= gop_length; ++idx) {
  memset(decision.sb_params_list, 0, count * sizeof(*decision.sb_params_list));
  codec_status = vp9_extrc_get_encodeframe_decision(ext_ratectrl, idx, &decision);
}
```
