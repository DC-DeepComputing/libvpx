---
name: libvpx/vpxworker-hook-return-boolean-success
description: When implementing or updating VPxWorkerHook multithreaded worker callbacks, ensure they return 1 (true) on success and 0 (false) on failure
rubric: correctness
severity: should_fix
---

**Rule.** When implementing callback functions assigned to `VPxWorkerHook` or
passed to worker dispatchers, return `1` (true) on successful execution and `0`
(false) upon error. Do not return `0` on success or use 0-as-success status
codes in worker hooks.

**Why.** `VPxWorker` records worker failure via `worker->had_error |=
!worker->hook(data1, data2)` and checks `!worker->had_error` during `sync()`.
Returning `0` from a worker hook indicates a failure to the worker runner,
causing synchronization and subsequent frame processing to fail.

## Examples

Bad:

```cpp
static int enc_row_mt_worker_hook(void *arg1, void *arg2) {
  EncWorkerData *const data = (EncWorkerData *)arg1;
  VP9_COMP *const cpi = (VP9_COMP *)arg2;
  // Encode superblock rows across threads...
  if (encode_row_mt(cpi, data) != 0) {
    return 1;  // Bug: returning non-zero signals error in POSIX style
  }
  return 0;  // Bug: returning 0 falsely signals worker error to Sync()
}
```

Good:

```cpp
static int enc_row_mt_worker_hook(void *arg1, void *arg2) {
  EncWorkerData *const data = (EncWorkerData *)arg1;
  VP9_COMP *const cpi = (VP9_COMP *)arg2;
  // Encode superblock rows across threads...
  if (encode_row_mt(cpi, data) != 0) {
    return 0;  // Return 0 (false) to signal error
  }
  return 1;  // Return 1 (true) to signal success
}
```
