---
name: libvpx/check-null-in-deallocation-and-cleanup-functions
description: When defining custom deallocation or resource cleanup functions, return void and check input pointers for NULL internally to allow unconditional caller invocation
rubric: correctness
severity: should_fix
---

**Rule.** When defining custom deallocation, free, teardown, or resource cleanup
functions (such as `*_free`, `*_destroy`, or `*_cleanup` helpers like
`vpx_free_tpl_gop_stats` or `vp9_dec_free_row_mt_mem`):

1. Use a `void` return type rather than returning unrecoverable error or status codes (such as `vpx_codec_err_t` or `int`), as cleanup failures cannot be meaningfully recovered by callers.
2. Check input pointers for `NULL` internally and return early or no-op before dereferencing struct members, buffer arrays, or inner handles.
3. Ensure cleanup helpers safely tolerate `NULL` pointers so that callers and error-unwinding paths can invoke them unconditionally without redundant external `NULL` guards.

**Why.** Standard C cleanup semantics (like `free(NULL)` and `vpx_free(NULL)`)
allow callers and error-unwinding paths to invoke deallocators unconditionally
without manual NULL checks. Returning status or error codes from deallocators
clutters the API surface and imposes unnecessary error-handling boilerplate on
callers for unrecoverable failures. Omitting internal NULL checks causes
null-pointer dereferences when cleaning up failed or partially initialized
structures and forces callers to write redundant defensive guards.

## Examples

Bad:

```cpp
// Anti-pattern 1: Returning status code and dereferencing pointer without checking NULL
vpx_codec_err_t vpx_free_tpl_gop_stats(VpxTplGopStats *data) {
  int frame;
  for (frame = 0; frame < data->size; frame++) {
    free(data->frame_stats_list[frame].block_stats_list);
  }
  free(data->frame_stats_list);
  return VPX_CODEC_OK;
}

// Anti-pattern 2: Caller forced to write redundant NULL guard
if (pbi->row_mt_worker_data != NULL) {
  vp9_dec_free_row_mt_mem(pbi->row_mt_worker_data);
}
```

Good:

```cpp
// Cleanup function returns void and safely returns early on NULL
void vpx_free_tpl_gop_stats(VpxTplGopStats *data) {
  int frame;
  if (data == NULL) return;
  for (frame = 0; frame < data->size; frame++) {
    free(data->frame_stats_list[frame].block_stats_list);
  }
  free(data->frame_stats_list);
}

// Callers can invoke cleanup unconditionally without external NULL check
vp9_dec_free_row_mt_mem(pbi->row_mt_worker_data);
```
