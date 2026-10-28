---
name: libvpx/avoid-pthread-mutex-initializer-and-static-mutexes
description: When adding or using pthread_mutex_t synchronization in libvpx, allocate mutexes dynamically in context structs and initialize with pthread_mutex_init instead of static PTHREAD_MUTEX_INITIALIZER
rubric: correctness
severity: should_fix
---

**Rule.** Do not use PTHREAD_MUTEX_INITIALIZER or declare static pthread_mutex_t
variables. Store mutexes as fields within dynamic context structures (such as
VP9_COMP or worker data structures) and manage their lifecycle explicitly using
pthread_mutex_init() and pthread_mutex_destroy().

**Why.** libvpx implements a Windows compatibility layer
(vpx_util/vpx_pthread.h) mapping pthread_mutex_t to Win32 synchronization
primitives (such as SRWLOCK), which do not support POSIX
PTHREAD_MUTEX_INITIALIZER static initialization and will break Windows builds.
Additionally, static mutexes introduce global serialization and race hazards
across concurrent encoder or decoder instances.

## Examples

Bad:

```cpp
// Static mutex with POSIX static initializer breaks on Windows and serializes instances.
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void encode_cluster(...) {
  pthread_mutex_lock(&mutex);
  // ... critical section ...
  pthread_mutex_unlock(&mutex);
}
```

Good:

```cpp
// Store mutex inside the context structure and initialize dynamically.
struct VP9_COMP {
  pthread_mutex_t kmeans_mutex;
  // ...
};

void vp9_create_compressor(VP9_COMP *cpi) {
  pthread_mutex_init(&cpi->kmeans_mutex, NULL);
}

void vp9_remove_compressor(VP9_COMP *cpi) {
  pthread_mutex_destroy(&cpi->kmeans_mutex);
}

void encode_cluster(VP9_COMP *cpi) {
  pthread_mutex_lock(&cpi->kmeans_mutex);
  // ... critical section ...
  pthread_mutex_unlock(&cpi->kmeans_mutex);
}
```
