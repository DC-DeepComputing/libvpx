---
name: libvpx/use-size-t-for-buffer-and-allocation-sizes
description: Use size_t rather than int, uint32_t, or explicit casts for buffer lengths, frame byte sizes, allocation capacities, and sizeof() expressions
rubric: readability
severity: should_fix
---

**Rule.** Use `size_t` rather than `int`, `uint32_t`, or `unsigned` for
variables, struct fields (e.g., `buffer_alloc_sz`, `frame_size`), function
parameters, and return types representing memory buffer capacities, frame byte
dimensions, allocation sizes, and byte counts—especially when computed using
`sizeof()` or passed to memory management routines (`vpx_memalign`,
`vpx_malloc`, `malloc`, `memset`, `memcpy`). Avoid casting signed integer size
variables or struct fields to or from `size_t` at comparison, assignment, or
allocation call sites; update the underlying declarations and struct members to
`size_t` instead.

**Why.** Standard memory management APIs (`vpx_memalign`, `malloc`, `memset`,
`memcpy`) and the `sizeof()` operator accept and return `size_t`. Storing buffer
and frame byte sizes in signed `int` or 32-bit types risks integer overflow and
truncation for large frame buffers or high-resolution video on 64-bit systems.
Consistently typing byte sizes as `size_t` eliminates signed/unsigned mismatch
warnings and removes the need for explicit narrowing or widening type casts.

## Examples

Bad:

```cpp
// Bad: Storing allocation sizes in signed int and using explicit casts
struct yv12_buffer_config {
  int buffer_alloc_sz;
  int frame_size;
};

const int dqcoeff_size =
    (num_sbs << DQCOEFFS_PER_SB_LOG2) * sizeof(*row_mt_handle->dqcoeff[0]);
row_mt_worker_data->dqcoeff[plane] = vpx_memalign(32, dqcoeff_size);

if (frame_size > (size_t)ybf->buffer_alloc_sz) {
  ybf->buffer_alloc_sz = (size_t)frame_size;
}
ybf->frame_size = (int)frame_size;
```

Good:

```cpp
// Good: Declare buffer and frame sizes as size_t without explicit casts
struct yv12_buffer_config {
  size_t buffer_alloc_sz;
  size_t frame_size;
};

const size_t dqcoeff_size =
    ((size_t)num_sbs << DQCOEFFS_PER_SB_LOG2) *
    sizeof(*row_mt_handle->dqcoeff[0]);
row_mt_worker_data->dqcoeff[plane] = vpx_memalign(32, dqcoeff_size);

if (frame_size > ybf->buffer_alloc_sz) {
  ybf->buffer_alloc_sz = frame_size;
}
ybf->frame_size = frame_size;
```
