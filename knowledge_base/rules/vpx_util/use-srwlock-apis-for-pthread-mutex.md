---
name: libvpx/vpx_util/use-srwlock-apis-for-pthread-mutex
description: When implementing Windows pthread mutex wrappers backed by SRWLOCK, use Win32 SRWLock functions like TryAcquireSRWLockExclusive instead of CRITICAL_SECTION APIs
rubric: correctness
severity: blocking
---

**Rule.** When implementing POSIX thread emulation or synchronization wrappers
on Windows backed by SRWLOCK (pthread_mutex_t), ensure all mutex operations
(such as pthread_mutex_trylock, pthread_mutex_lock, pthread_mutex_unlock, and
pthread_mutex_init) consistently call Win32 SRWLock APIs (e.g.,
TryAcquireSRWLockExclusive, AcquireSRWLockExclusive, ReleaseSRWLockExclusive,
InitializeSRWLock). Never pass an SRWLOCK pointer to CRITICAL_SECTION APIs like
TryEnterCriticalSection.

**Why.** SRWLOCK and CRITICAL_SECTION have incompatible internal memory layouts
and locking protocols. Passing an SRWLOCK pointer to CRITICAL_SECTION functions
causes type confusion, memory corruption, and runtime crashes.

## Examples

Bad:

```cpp
static INLINE int pthread_mutex_trylock(pthread_mutex_t *const mutex) {
  // Bug: pthread_mutex_t is typedef'd to SRWLOCK, but TryEnterCriticalSection expects CRITICAL_SECTION*.
  return TryEnterCriticalSection(mutex) ? 0 : EBUSY;
}
```

Good:

```cpp
static INLINE int pthread_mutex_trylock(pthread_mutex_t *const mutex) {
  return TryAcquireSRWLockExclusive(mutex) ? 0 : EBUSY;
}
```
