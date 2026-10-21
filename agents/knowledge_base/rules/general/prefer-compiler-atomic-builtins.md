---
name: libvpx/prefer-compiler-atomic-builtins
rubric: safety
severity: should_fix
---

**Rule.** When implementing portable atomic operations and synchronization
primitives, prefer compiler builtins (such as GCC/Clang `__atomic_load_n` and
`__atomic_store_n`) guarded by `__has_builtin` or compiler version checks rather
than including TSan-specific headers (`<sanitizer/tsan_interface_atomic.h>`) or
C11 `<stdatomic.h>`. Provide fallback memory barriers for compilers without
builtin support.

**Why.** `<stdatomic.h>` is only guaranteed in C11 and is not supported
uniformly across C99, MSVC, or older GCC/G++ configurations. Compiler
`__atomic_*` builtins work across C and C++ modes without header compatibility
issues and are natively understood by ThreadSanitizer without dedicated TSan
wrapper code.

## Examples

Bad:

```cpp
#if defined(__has_feature) && __has_feature(thread_sanitizer)
#include <sanitizer/tsan_interface_atomic.h>
static INLINE int vpx_atomic_load_relaxed(volatile atomic_int *x) {
  return __tsan_atomic32_load(x, __tsan_memory_order_relaxed);
}
#endif
```

Good:

```cpp
#if !defined(__has_builtin)
#define __has_builtin(x) 0
#endif

#if (__has_builtin(__atomic_load_n)) || \
    (defined(__GNUC__) && (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 7)))
static INLINE int vpx_atomic_load_acquire(const vpx_atomic_int *atomic) {
  return __atomic_load_n(&atomic->value, __ATOMIC_ACQUIRE);
}
#endif
```
