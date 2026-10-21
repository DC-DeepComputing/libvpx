---
name: libvpx/free-allocated-struct-members-in-teardown
rubric: safety
severity: should_fix
---

**Rule.** When adding heap-allocated members or dynamic buffers to context,
state, worker, or helper structs (such as `VP9Decoder`, `VP9_COMP`,
`MotionField`, or worker thread data), ensure every allocated field is
deallocated with `vpx_free()` in corresponding cleanup, teardown, or removal
functions (such as `vp9_decoder_remove()`, `vp9_remove_compressor()`, or
`*_free()` / `*_destroy()` helpers) before freeing the parent struct.
Additionally, ensure all early-return and error cleanup paths within allocation
or initialization helpers deallocate any buffers allocated prior to failure to
prevent memory leaks.

**Why.** In C, dynamically allocated struct members persist across frames and
operations without automatic destructor cleanup. Failing to free all allocated
member pointers during struct teardown or upon partial initialization failure
permanently leaks memory whenever codecs, contexts, or worker pools are
reconfigured, destroyed, or fail to initialize.

## Examples

Bad:

```cpp
void vp9_decoder_remove(VP9Decoder *pbi) {
  vpx_free(pbi->tile_worker_data);
  vpx_free(pbi->tile_workers);
  // Missing deallocation of pbi->thread_data
  vp9_remove_common(&pbi->common);
  vpx_free(pbi);
}

void vp9_free_motion_field(MotionField *motion_field) {
  vpx_free(motion_field->mf);
  // Missing deallocation of motion_field->set_mv
  vpx_free(motion_field->local_structure);
  vp9_zero(*motion_field);
}
```

Good:

```cpp
void vp9_decoder_remove(VP9Decoder *pbi) {
  vpx_free(pbi->tile_worker_data);
  vpx_free(pbi->tile_workers);
  vpx_free(pbi->thread_data);
  vp9_remove_common(&pbi->common);
  vpx_free(pbi);
}

void vp9_free_motion_field(MotionField *motion_field) {
  vpx_free(motion_field->mf);
  vpx_free(motion_field->set_mv);
  vpx_free(motion_field->local_structure);
  vp9_zero(*motion_field);
}
```
