---
name: libvpx/no-cleanup-after-vpx-internal-error
rubric: safety
severity: should_fix
---

**Rule.** Do not place cleanup logic, counter updates, or loop break statements
after vpx_internal_error(), as it performs a longjmp and never returns to caller
code. Ensure all state and resource counters needed for teardown (such as
allocated thread or event counts) are updated before invoking
vpx_internal_error(). When allocating resources in a loop, destroy any partially
initialized resources from the failing iteration and break out of the loop to
record the count of completed iterations before triggering error handling or
calling vpx_internal_error().

**Why.** In libvpx, vpx_internal_error() uses longjmp() to jump directly to the
registered error handler frame and never returns to the caller. Any statements
placed after it are unreachable, leaving resource counters out of date and
causing teardown routines to skip cleanup or read invalid state. Additionally,
if an iteration fails after only some sub-resources (such as synchronization
semaphores or mutexes) are initialized, those sub-resources leak permanently
unless explicitly destroyed before exiting or breaking out of the loop.

## Examples

Bad:

```cpp
for (ithread = 0; ithread < pbi->decoding_thread_count; ++ithread) {
  if (sem_init(&pbi->h_event_start[ithread], 0, 0) ||
      sem_init(&pbi->h_event_end[ithread], 0, 0)) {
    // Leaks h_event_start on failure and longjmps before updating counter.
    vpx_internal_error(&pbi->common.error, VPX_CODEC_MEM_ERROR, "Init failed");
    break;  // Unreachable code after longjmp.
  }
}
pbi->allocated_decoding_thread_count = ithread;
```

Good:

```cpp
for (ithread = 0; ithread < pbi->decoding_thread_count; ++ithread) {
  if (sem_init(&pbi->h_event_start[ithread], 0, 0)) break;
  if (sem_init(&pbi->h_event_end[ithread], 0, 0)) {
    sem_destroy(&pbi->h_event_start[ithread]);
    break;
  }
}
pbi->allocated_decoding_thread_count = ithread;
if (pbi->allocated_decoding_thread_count != pbi->decoding_thread_count) {
  vpx_internal_error(&pbi->common.error, VPX_CODEC_MEM_ERROR, "Init failed");
}
```
