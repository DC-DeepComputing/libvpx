---
name: libvpx/define-and-undef-local-feature-macros
description: 'When defining local helper macros or feature checks, consolidate duplicate preprocessor toggles into a single shared macro and #undef it at scope exit'
rubric: readability
severity: nit
---

**Rule.** When defining temporary helper macros, compiler capability/sanitizer
checks (e.g., `__has_feature`, `__has_builtin`), or configuration toggles within
a function or file, consolidate redundant `#if` checks into a single shared
helper macro rather than duplicating nested preprocessor guards. Always `#undef`
temporary helper macros at the end of the enclosing block, function, or file to
prevent macro namespace pollution.

**Why.** C and C++ preprocessor macros do not respect block or function scopes
and persist across the entire translation unit once defined, risking unintended
identifier collisions and macro leakage. Consolidating repeated nested feature
checks or redundant toggle definitions into a single scoped macro improves
readability, prevents configuration drift, and keeps the macro namespace clean.

## Examples

Bad:

```cpp
// Variant 1: Duplicating nested compiler/sanitizer checks across functions
static INLINE int protected_read(pthread_mutex_t *const mutex, const int *p) {
#if defined(__has_feature)
#if __has_feature(thread_sanitizer)
  int ret;
  pthread_mutex_lock(mutex);
  ret = *p;
  pthread_mutex_unlock(mutex);
  return ret;
#endif
#endif
  return *p;
}

// Variant 2: Defining redundant local toggles and leaking into the translation unit
void transform_stage5(void) {
#define AVOID_OVERFLOW_1 (1)
#define AVOID_OVERFLOW_2 (1)
#if AVOID_OVERFLOW_1
  step_one();
#endif
  step_two();
  // AVOID_OVERFLOW_* macros remain defined after function exit
}
```

Good:

```cpp
// Variant 1: Consolidate repeated checks into a helper macro and #undef after use
#if defined(__has_feature)
#if __has_feature(thread_sanitizer)
#define USE_MUTEX_LOCK 1
#endif
#endif

static INLINE int protected_read(pthread_mutex_t *const mutex, const int *p) {
#if defined(USE_MUTEX_LOCK)
  int ret;
  pthread_mutex_lock(mutex);
  ret = *p;
  pthread_mutex_unlock(mutex);
  return ret;
#endif
  return *p;
}
#undef USE_MUTEX_LOCK

// Variant 2: Consolidate redundant toggles and #undef at scope exit
void transform_stage5(void) {
#define AVOID_OVERFLOW (1)
#if AVOID_OVERFLOW
  step_one();
#endif
  step_two();
#undef AVOID_OVERFLOW
}
```
