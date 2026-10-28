---
name: libvpx/guard-platform-threading-changes
description: When altering platform-specific threading, synchronization, or scheduling behavior, guard OS-specific optimizations with explicit platform macros and retain portable fallbacks
rubric: correctness
severity: should_fix
---

**Rule.** When modifying threading, synchronization, or task scheduling
primitives (such as yield, sleep, or spin-wait operations), guard OS-specific
optimizations or no-op elisions with explicit operating system preprocessor
checks (e.g., defined(__unix__) || defined(__APPLE__)). Always maintain a
portable fallback path (such as calling sched_yield()) for other POSIX or
generic targets instead of unconditionally altering behavior across all
non-Windows platforms.

**Why.** Scheduler preemption guarantees and thread scheduling semantics differ
significantly across operating systems and kernels. An optimization or
assumption valid for Linux or macOS (such as thread preemption during spin-waits
making explicit yielding unnecessary) can cause CPU starvation or thread lockups
on other POSIX platforms or RTOS environments if applied indiscriminately.

## Examples

Bad:

```cpp
/* Unconditionally eliding yield across all non-Windows POSIX platforms */
#if defined(_WIN32) && !HAVE_PTHREAD_H
#define thread_sleep(nms) Sleep(nms)
#else
#define thread_sleep(nms)
#endif
```

Good:

```cpp
/* Guard OS-specific yield elision and preserve portable sched_yield fallback */
#if defined(_WIN32) && !HAVE_PTHREAD_H
#define thread_sleep(nms) Sleep(nms)
#elif defined(__unix__) || defined(__APPLE__)
#define thread_sleep(nms)
#else
#define thread_sleep(nms) sched_yield()
#endif
```
