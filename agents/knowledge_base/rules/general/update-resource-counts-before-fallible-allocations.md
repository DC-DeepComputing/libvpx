---
name: libvpx/update-resource-counts-before-fallible-allocations
rubric: safety
severity: should_fix
---

**Rule.** Assign struct count, capacity, and size fields before allocating
arrays or initializing elements with fallible allocation macros like
CHECK_MEM_ERROR(). Ensure cleanup and teardown routines invoked during
allocation failures or error longjmps have accurate counts to properly free and
destroy allocated resources.

**Why.** In libvpx, allocation macros such as CHECK_MEM_ERROR() invoke
vpx_internal_error() and trigger an immediate setjmp/longjmp error bailout upon
failure. If struct count fields are only updated after multiple allocations or
initializations, an allocation failure midway executes teardown handlers with
stale or uninitialized counts, leaking previously allocated memory or skipping
mutex/cond destruction.

## Examples

Bad:

```cpp
CHECK_MEM_ERROR(cm, worker_data->recon_sync_mutex,
                vpx_malloc(sizeof(*worker_data->recon_sync_mutex) * num_jobs));
for (i = 0; i < num_jobs; ++i) {
  pthread_mutex_init(&worker_data->recon_sync_mutex[i], NULL);
}
CHECK_MEM_ERROR(cm, worker_data->recon_sync_cond,
                vpx_malloc(sizeof(*worker_data->recon_sync_cond) * num_jobs));
worker_data->num_jobs = num_jobs;
```

Good:

```cpp
worker_data->num_jobs = num_jobs;
CHECK_MEM_ERROR(cm, worker_data->recon_sync_mutex,
                vpx_malloc(sizeof(*worker_data->recon_sync_mutex) * num_jobs));
for (i = 0; i < num_jobs; ++i) {
  pthread_mutex_init(&worker_data->recon_sync_mutex[i], NULL);
}
CHECK_MEM_ERROR(cm, worker_data->recon_sync_cond,
                vpx_malloc(sizeof(*worker_data->recon_sync_cond) * num_jobs));
```
