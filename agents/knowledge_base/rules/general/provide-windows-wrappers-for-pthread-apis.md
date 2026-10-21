---
name: libvpx/provide-windows-wrappers-for-pthread-apis
rubric: correctness
severity: blocking
---

**Rule.** When introducing calls to POSIX threading functions or types
(pthread_*), ensure corresponding non-POSIX / Windows Win32 emulation wrappers
and type definitions are provided in vpx_util/vpx_pthread.h (or
vpx_util/vpx_thread.h). Do not invoke POSIX thread APIs directly without Windows
emulation fallbacks.

**Why.** libvpx supports non-POSIX platforms such as Windows (MSVC) where native
pthread.h is unavailable by providing inline Win32/condition variable emulation
wrappers in vpx_util/vpx_pthread.h. Calling a POSIX thread API or using a
pthread type that lacks an emulation wrapper breaks builds on Windows.

## Examples

Bad:

```cpp
/* Calling pthread_cond_broadcast across codecs without a Windows wrapper */
#include "vpx_util/vpx_pthread.h"

void signal_row_complete(VP9LfSync *lf_sync, int row) {
  pthread_cond_broadcast(&lf_sync->recon_done_cond[row]);
}
```

Good:

```cpp
/* In vpx_util/vpx_pthread.h: provide Windows emulation wrapper */
#if defined(_WIN32) && !HAVE_PTHREAD_H
static INLINE int pthread_cond_broadcast(pthread_cond_t *const condition) {
  WakeAllConditionVariable(condition);
  return 0;
}
#endif

/* Call site in codec code */
void signal_row_complete(VP9LfSync *lf_sync, int row) {
  pthread_cond_broadcast(&lf_sync->recon_done_cond[row]);
}
```
