---
name: libvpx/vp9/queue-dependent-jobs-before-signaling-completion-map
description: When updating row-multithreading dependency maps with map_write(), enqueue any dependent follow-up jobs (such as loop-filter jobs) before signaling completion to prevent race conditions
rubric: safety
severity: should_fix
---

**Rule.** In multithreaded pipelines using dependency or completion maps
(map_write()/map_read()), always enqueue any dependent follow-up jobs (such as
loop filter jobs via vp9_jobq_queue()) before writing completion status to the
dependency map.

**Why.** Signaling task completion via map_write() immediately unblocks waiting
worker threads on dependent rows. If downstream jobs are queued after signaling
completion, dependent worker threads may finish their work and queue subsequent
jobs first, causing out-of-order execution and race conditions.

## Examples

Bad:

```cpp
recon_partition(tile_data_recon, pbi, mi_row, mi_col, BLOCK_64X64, 4);
// Signaling RECON_DONE before queuing LPF_JOB allows dependent rows to race
map_write(row_mt_worker_data, (cur_sb_row * sb_cols) + c, RECON_DONE);
if (is_lpf_job_ready) {
  vp9_jobq_queue(&row_mt_worker_data->jobq, &lpf_job, sizeof(lpf_job));
}
```

Good:

```cpp
recon_partition(tile_data_recon, pbi, mi_row, mi_col, BLOCK_64X64, 4);
if (is_lpf_job_ready) {
  vp9_jobq_queue(&row_mt_worker_data->jobq, &lpf_job, sizeof(lpf_job));
}
// Signal RECON_DONE only after any dependent job is queued
map_write(row_mt_worker_data, (cur_sb_row * sb_cols) + c, RECON_DONE);
```
