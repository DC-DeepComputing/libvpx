---
name: libvpx/separate-thread-signaling-from-atomic-stores
description: When defining or wrapping atomic synchronization operations, keep thread signaling and futex wakes separate from basic atomic store and load functions
rubric: design
severity: nit
---

**Rule.** Do not embed thread signaling mechanisms (such as futex wake syscalls,
event triggers, or condition variable signals) directly inside basic atomic
store or load primitives. Provide separate, explicit signaling helper functions
(such as vp8_atomic_spin_signal()) so callers can update memory state without
unnecessary syscall overhead and selectively control when to wake waiting
threads.

**Why.** Inlining thread-signaling syscalls like futex wake into atomic store
operations couples memory visibility with thread notification, imposing heavy
syscall overhead on every store even when no thread is waiting, and prevents
callers from batching stores before signaling.

## Examples

Bad:

```cpp
static INLINE void vpx_atomic_store_release(vpx_atomic_int *atomic, int value) {
  __atomic_store_n(&atomic->value, value, __ATOMIC_RELEASE);
  if (atomic->has_futex) {
    syscall(SYS_futex, &atomic->value, FUTEX_WAKE_PRIVATE, INT_MAX, NULL, NULL, 0);
  }
}
```

Good:

```cpp
static INLINE void vpx_atomic_store_release(vpx_atomic_int *atomic, int value) {
  __atomic_store_n(&atomic->value, value, __ATOMIC_RELEASE);
}

/* Explicit signaling helper allows selective wakes without store overhead */
static INLINE void vp8_atomic_spin_signal(vpx_atomic_int *atomic) {
  if (atomic->has_futex) {
    syscall(SYS_futex, &atomic->value, FUTEX_WAKE_PRIVATE, INT_MAX, NULL, NULL, 0);
  }
}
```
