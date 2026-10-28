---
name: libvpx/vp8/synchronize-worker-threads-before-buffer-consumers
description: When background worker threads mutate frame buffers, wait for worker completion unconditionally before downstream metric calculation, packet generation, or statistics blocks
rubric: safety
severity: should_fix
---

**Rule.** Wait for asynchronous background worker threads (such as
multi-threaded loopfiltering) to complete before entering any downstream blocks
(such as PSNR calculation, packet generation, or internal stats logging) that
inspect reconstructed frame buffers. Do not duplicate synchronization waits
inside individual conditional branches or defer worker waiting past
buffer-reading consumers.

**Why.** Downstream metric calculation, packet creation, and debug statistics
read the reconstructed frame buffers. If worker thread completion is not waited
on prior to entering these blocks, or if waiting is conditioned on a flag that
only covers one consumer path, unsynchronized reads cause data races and
non-deterministic metric corruption across other consumers.

## Examples

Bad:

```cpp
if (cpi->b_calculate_psnr && cpi->pass != 1 && cm->show_frame) {
#if CONFIG_MULTITHREAD
  /* Thread sync duplicated or placed inside one conditional consumer */
  if (vpx_atomic_load_acquire(&cpi->b_multi_threaded) && cpi->b_lpf_running) {
    vp8_sem_wait(&cpi->h_event_end_lpf);
    cpi->b_lpf_running = 0;
  }
#endif
  generate_psnr_packet(cpi);
}

#if CONFIG_INTERNAL_STATS
  /* Unsynchronized buffer read causes data race */
#endif
```

Good:

```cpp
#if CONFIG_MULTITHREAD
  /* Unconditionally synchronize worker thread before all consumers */
  if (vpx_atomic_load_acquire(&cpi->b_multi_threaded) && cpi->b_lpf_running) {
    vp8_sem_wait(&cpi->h_event_end_lpf);
    cpi->b_lpf_running = 0;
  }
#endif

  if (cpi->b_calculate_psnr && cpi->pass != 1 && cm->show_frame) {
    generate_psnr_packet(cpi);
  }

#if CONFIG_INTERNAL_STATS
  /* Safely reads reconstructed frame buffer */
#endif
```
