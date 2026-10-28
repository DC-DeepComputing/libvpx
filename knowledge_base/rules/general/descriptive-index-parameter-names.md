---
name: libvpx/descriptive-index-parameter-names
description: When a function takes multiple index parameters for distinct structures or arrays, use descriptive names identifying each target instead of generic identifiers
rubric: readability
severity: nit
---

**Rule.** When defining functions that take multiple index, row, column, or
offset parameters addressing different data structures, buffers, or
synchronization arrays, use descriptive parameter names that explicitly identify
their target (e.g., map_idx, sync_idx, tile_row, frame_row). Avoid generic,
single-letter, or ambiguous identifiers (such as idx, r, i, or j) for distinct
indices across different structures.

**Why.** Generic or single-letter parameter names obscure which structure,
buffer, or lock array each parameter indexes into, making function definitions
harder to understand and significantly increasing the risk of argument
transposition bugs at call sites.

## Examples

Bad:

```cpp
static void map_write(RowMTWorkerData *const row_mt_worker_data, int idx,
                      int r) {
  pthread_mutex_lock(&row_mt_worker_data->recon_sync_mutex[r]);
  row_mt_worker_data->recon_map[idx] = 1;
  pthread_cond_signal(&row_mt_worker_data->recon_sync_cond[r]);
  pthread_mutex_unlock(&row_mt_worker_data->recon_sync_mutex[r]);
}
```

Good:

```cpp
static void map_write(RowMTWorkerData *const row_mt_worker_data, int map_idx,
                      int sync_idx) {
  pthread_mutex_lock(&row_mt_worker_data->recon_sync_mutex[sync_idx]);
  row_mt_worker_data->recon_map[map_idx] = 1;
  pthread_cond_signal(&row_mt_worker_data->recon_sync_cond[sync_idx]);
  pthread_mutex_unlock(&row_mt_worker_data->recon_sync_mutex[sync_idx]);
}
```
