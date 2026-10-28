---
name: libvpx/vpx_util/no-unused-compatibility-shims
description: When implementing platform compatibility wrappers or shims in utility headers, omit unused typedefs, dummy functions, and stubs not required by the codebase
rubric: readability
severity: nit
---

**Rule.** Do not introduce unused compatibility typedefs, dummy wrappers, or
stub functions (such as pthread_attr_t and its init/destroy helpers) when
implementing platform abstraction layers. Only implement the minimal set of
types and wrappers actually used by the codebase, using generic parameter types
(like const void *) where unused parameters must be accepted.

**Why.** Unused compatibility shims and dummy functions create dead code,
increase maintenance overhead, and bloat platform headers without providing any
functional benefit to the codebase.

## Examples

Bad:

```cpp
typedef int pthread_attr_t;

static INLINE int pthread_attr_init(pthread_attr_t *attr) {
  (void)attr;
  return 0;
}

static INLINE int pthread_attr_destroy(pthread_attr_t *attr) {
  (void)attr;
  return 0;
}
```

Good:

```cpp
// Only define wrappers and types actively used by the codebase.
static INLINE int pthread_create(pthread_t *const thread, const void *attr,
                                 unsigned int(__stdcall *start)(void *),
                                 void *arg) {
  (void)attr;
  *thread = (pthread_t)_beginthreadex(NULL, 0, start, arg, 0, NULL);
  return *thread ? 0 : 1;
}
```
