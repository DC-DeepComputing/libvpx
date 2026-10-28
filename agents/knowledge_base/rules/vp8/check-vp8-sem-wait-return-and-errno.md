---
name: libvpx/vp8/check-vp8-sem-wait-return-and-errno
description: When checking return status or retry loops for vp8_sem_wait or sem_wait, verify failure using != 0 instead of == -1 and reset errno = 0 before testing EINTR
rubric: correctness
severity: should_fix
---

**Rule.** When checking return status or handling retry loops for vp8_sem_wait()
or sem_wait() in VP8 threading routines, test for failure using != 0 (or success
using == 0) rather than checking == -1. When checking errno == EINTR around
semaphore operations, explicitly initialize errno = 0 before the call to avoid
evaluating stale errno values on non-POSIX platforms.

**Why.** In vp8/common/threading.h, vp8_sem_wait() abstracts platform
differences: Windows wrappers use WaitForSingleObject() (returning 1 on failure
and 0 on success) and Apple wrappers use semaphore_wait() (returning
kern_return_t error codes). Checking == -1 fails to detect non-POSIX failure
codes, and non-POSIX platforms do not set errno, so uninitialized errno checks
can evaluate stale values.

## Examples

Bad:

```cpp
// Fails to detect failures on Windows (returns 1) and Apple (returns kern_return_t),
// and risks evaluating stale errno on non-POSIX platforms.
while (vp8_sem_wait(&cpi->h_event_end_lpf) == -1 && errno == EINTR) {
}
```

Good:

```cpp
// Correctly handles non-zero error codes across all platforms and clears errno.
errno = 0;
while (vp8_sem_wait(&cpi->h_event_end_lpf) != 0 && errno == EINTR) {
}
```
