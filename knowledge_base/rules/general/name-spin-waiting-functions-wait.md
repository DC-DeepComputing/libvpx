---
name: libvpx/name-spin-waiting-functions-wait
description: When defining synchronization helpers that loop or spin-wait on a condition, use 'wait' or 'spin_wait' in the function name rather than 'read' or 'sync_read'
rubric: readability
severity: should_fix
---

**Rule.** Name synchronization functions that loop or spin-wait for a condition
with 'wait' or 'spin_wait' (e.g., vp8_atomic_spin_wait) rather than non-blocking
terms such as 'read' or 'sync_read'. Reserve 'read' or 'load' for non-blocking
single atomic read operations.

**Why.** Function names containing 'read' or 'sync_read' misleadingly imply an
instantaneous, non-blocking atomic load with memory ordering (analogous to
compiler atomic built-ins). Including 'wait' or 'spin_wait' clearly signals that
the function loops or blocks until a synchronization condition is met.

## Examples

Bad:

```cpp
// Misleading name suggests an instantaneous atomic read rather than a blocking loop.
static INLINE void vp8_atomic_sync_read(int mb_col,
                                        const vpx_atomic_int *last_row,
                                        int nsync) {
  while (mb_col > (vpx_atomic_load_acquire(last_row) - nsync)) {
    x86_pause_hint();
    thread_sleep(0);
  }
}
```

Good:

```cpp
// Accurately communicates that the function spin-waits until the condition is met.
static INLINE void vp8_atomic_spin_wait(int mb_col,
                                        const vpx_atomic_int *last_row,
                                        int nsync) {
  while (mb_col > (vpx_atomic_load_acquire(last_row) - nsync)) {
    x86_pause_hint();
    thread_sleep(0);
  }
}
```
