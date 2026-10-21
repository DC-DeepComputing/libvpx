---
name: libvpx/check-allocation-return-for-null
rubric: safety
severity: should_fix
---

**Rule.** Always verify the return value or status code of dynamic memory
allocation functions (such as vpx_malloc(), vpx_calloc(), vpx_memalign(),
vpx_realloc(), malloc(), calloc(), or realloc()) and resource/buffer allocation
helpers (such as vp9_alloc_*() or vp8_alloc_*()) for NULL or failure status
(STATUS_FAILED, non-zero error) before accessing allocated resources. In
functions returning a status code or error context, handle allocation failures
gracefully by cleaning up partially allocated resources and returning an
appropriate error code (such as VPX_CODEC_MEM_ERROR or STATUS_FAILED), or by
invoking vpx_internal_error(). Do not use assertions (such as assert(0) or
assert(ptr != NULL)) to catch runtime memory allocation failures.

Additionally, when implementing allocation helper functions, update struct lifecycle or state flags (such as allocated = 1, ready = 1, or inited = 1) only after all allocations and sub-initializations have completely succeeded.

**Why.** Dynamic memory allocations and buffer initializations can fail at
runtime due to memory exhaustion or invalid dimensions. Dereferencing unchecked
allocation pointers or ignoring allocation helper return codes leads to
null-pointer dereferences, data corruption, and undefined behavior. Setting
state flags like allocated = 1 before verifying allocations leaves data
structures partially initialized or corrupted if an allocation fails.
Propagating error status codes allows callers to abort gracefully and prevents
use-after-free or null pointer dereferences on half-allocated structs.
Furthermore, assert() is compiled out in release builds (NDEBUG), leaving
allocation failures completely unhandled in production, while in debug builds it
aborts execution instead of allowing the library and caller to recover cleanly.

## Examples

Bad:

```cpp
// Variant 1: Unchecked dynamic allocation before use
cm->postproc_state.generated_noise = vpx_calloc(
    generated_noise_size, sizeof(*cm->postproc_state.generated_noise));
vpx_setup_noise(sigma, cm->postproc_state.generated_noise,
                generated_noise_size);

// Variant 2: Setting state flag before verifying allocation helper results
void vp9_alloc_motion_field_info(MotionFieldInfo *motion_field_info, int n) {
  motion_field_info->allocated = 1;
  motion_field_info->mf_array =
      vpx_calloc(n, sizeof(*motion_field_info->mf_array));
  vp9_alloc_motion_field(motion_field_info->mf_array);
}

// Variant 3: Using assert() on memory allocation failure
motion_field->mf =
    vpx_calloc(block_rows * block_cols, sizeof(*motion_field_mf));
if (motion_field->mf == NULL) {
  assert(0);
  status = STATUS_FAILED;
}
```

Good:

```cpp
// Variant 1: Check allocation return pointer before use
cm->postproc_state.generated_noise = vpx_calloc(
    generated_noise_size, sizeof(*cm->postproc_state.generated_noise));
if (!cm->postproc_state.generated_noise) return 1;
vpx_setup_noise(sigma, cm->postproc_state.generated_noise,
                generated_noise_size);

// Variant 2: Propagate errors and set state flag only after allocations succeed
Status vp9_alloc_motion_field_info(MotionFieldInfo *motion_field_info, int n) {
  motion_field_info->mf_array =
      vpx_calloc(n, sizeof(*motion_field_info->mf_array));
  if (!motion_field_info->mf_array) return STATUS_FAILED;
  if (vp9_alloc_motion_field(motion_field_info->mf_array) != STATUS_OK) {
    return STATUS_FAILED;
  }
  motion_field_info->allocated = 1;
  return STATUS_OK;
}

// Variant 3: Gracefully handle allocation failure without assertions
motion_field->mf =
    vpx_calloc(block_rows * block_cols, sizeof(*motion_field->mf));
if (motion_field->mf == NULL) {
  status = STATUS_FAILED;
}
```
