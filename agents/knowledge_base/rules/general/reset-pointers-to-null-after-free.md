---
name: libvpx/reset-pointers-to-null-after-free
rubric: safety
severity: should_fix
---

**Rule.** When freeing dynamically allocated memory (via vpx_free(), free(),
delete, or custom destroy helpers) or destroying synchronization primitives
(such as pthread_cond_destroy(), pthread_mutex_destroy(), or sem_destroy()) held
in raw pointers or struct members, immediately reset the pointer or handle to
NULL (in C) or nullptr (in C++). Additionally, when freeing member buffers or
tearing down resources in surviving or reusable structs, reset all associated
allocation/validity flags (such as allocated, is_allocated), size, length, and
counter fields (such as allocated_tiles, allocated_decoding_thread_count, size,
count, num_jobs) to 0. Do not leave dangling non-NULL pointers, stale allocation
flags, or non-zero size counts after deallocation.

**Why.** Cleanup routines may be called repeatedly during codec reconfiguration,
reinitialization, error recovery, or partial teardown. Leaving non-NULL dangling
pointers, unreset allocation flags, or stale size/count fields causes subsequent
null checks, reallocation guards, debug assertions, or teardown loops to treat
freed resources as valid, leading to use-after-free memory corruption,
double-free crashes, or invalid handle dereferences.

## Examples

Bad:

```cpp
// Bad: dangling pointer after destroying synchronization primitives
if (row_mt_worker_data->recon_sync_cond != NULL) {
  for (i = 0; i < row_mt_worker_data->num_jobs; ++i) {
    pthread_cond_destroy(&row_mt_worker_data->recon_sync_cond[i]);
  }
  vpx_free(row_mt_worker_data->recon_sync_cond);
}

// Bad: member pointer and size left dangling after in-place buffer free
void vpx_free_tpl_gop_stats(VpxTplGopStats *tpl_gop_stats) {
  if (!tpl_gop_stats) return;
  for (int f = 0; f < tpl_gop_stats->size; ++f) {
    vpx_free(tpl_gop_stats->frame_stats_list[f].block_stats_list);
  }
  vpx_free(tpl_gop_stats->frame_stats_list);
}

// Bad: allocation flag left set after releasing buffer
void vp9_free_motion_field_info(MotionFieldInfo *motion_field_info) {
  if (motion_field_info->allocated) {
    vpx_free(motion_field_info->motion_field_array);
    motion_field_info->motion_field_array = NULL;
    motion_field_info->frame_num = 0;
    // Bug: motion_field_info->allocated is not reset to 0
  }
}
```

Good:

```cpp
// Good: reset handle pointer to NULL after free
if (row_mt_worker_data->recon_sync_cond != NULL) {
  for (i = 0; i < row_mt_worker_data->num_jobs; ++i) {
    pthread_cond_destroy(&row_mt_worker_data->recon_sync_cond[i]);
  }
  vpx_free(row_mt_worker_data->recon_sync_cond);
  row_mt_worker_data->recon_sync_cond = NULL;
}

// Good: reset pointer to NULL and size to 0 after in-place buffer free
void vpx_free_tpl_gop_stats(VpxTplGopStats *tpl_gop_stats) {
  if (!tpl_gop_stats) return;
  for (int f = 0; f < tpl_gop_stats->size; ++f) {
    vpx_free(tpl_gop_stats->frame_stats_list[f].block_stats_list);
  }
  vpx_free(tpl_gop_stats->frame_stats_list);
  tpl_gop_stats->frame_stats_list = NULL;
  tpl_gop_stats->size = 0;
}

// Good: reset pointer and allocation flag to 0
void vp9_free_motion_field_info(MotionFieldInfo *motion_field_info) {
  if (motion_field_info->allocated) {
    vpx_free(motion_field_info->motion_field_array);
    motion_field_info->motion_field_array = NULL;
    motion_field_info->frame_num = 0;
    motion_field_info->allocated = 0;
  }
}
```
