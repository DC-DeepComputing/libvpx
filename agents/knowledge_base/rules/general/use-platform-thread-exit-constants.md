---
name: libvpx/use-platform-thread-exit-constants
description: When defining cross-platform thread entry functions or threading wrappers, return explicit platform-specific exit constants like THREAD_EXIT_SUCCESS rather than pointer-to-integer casting macros
rubric: correctness
severity: should_fix
---

**Rule.** When defining cross-platform thread entry functions (such as THREADFN
worker callbacks) or threading wrapper macros across POSIX pthreads and Windows
threading APIs, use dedicated platform-specific exit constants like
THREAD_EXIT_SUCCESS (0 on Windows, NULL on POSIX) instead of wrapping thread
return values in pointer-to-integer casting macros.

**Why.** POSIX pthread entry routines return void*, whereas Windows
_beginthreadex functions return unsigned int. Casting pointer values to unsigned
int via generic return macros risks 64-bit pointer truncation on 64-bit Windows
(LLP64) and obscures caller intent when worker functions only need to return a
normal exit status.

## Examples

Bad:

```cpp
#if defined(_WIN32) && !HAVE_PTHREAD_H
#define THREADFN unsigned int __stdcall
#define THREAD_RETURN(val) (unsigned int)((DWORD_PTR)val)
#else
#define THREADFN void *
#define THREAD_RETURN(val) val
#endif

static THREADFN thread_loop(void *ptr) {
  // Worker loop execution...
  return THREAD_RETURN(NULL);
}
```

Good:

```cpp
#if defined(_WIN32) && !HAVE_PTHREAD_H
#define THREADFN unsigned int __stdcall
#define THREAD_EXIT_SUCCESS 0
#else
#define THREADFN void *
#define THREAD_EXIT_SUCCESS NULL
#endif

static THREADFN thread_loop(void *ptr) {
  // Worker loop execution...
  return THREAD_EXIT_SUCCESS;
}
```
