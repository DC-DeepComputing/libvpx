---
name: libvpx/assert-preconditions-outside-feature-guards
description: 'When asserting function preconditions or non-null pointers, place assertions outside feature #if guards if parameters are used across all build configurations'
rubric: correctness
severity: should_fix
---

**Rule.** Do not place function-level precondition assertions (such as non-null
pointer checks) inside feature-specific #if or #ifdef conditional compilation
blocks if the parameters are accessed or dereferenced across all build
configurations. Keep shared precondition assertions outside conditional
compilation guards so all build configurations validate function inputs.

**Why.** Placing precondition assertions inside feature #if guards (such as #if
CONFIG_MULTITHREAD) disables validation when compiling alternative
configurations, even though subsequent code in the function still accesses and
dereferences those arguments unconditionally.

## Examples

Bad:

```cpp
void vp9_loop_filter_dealloc(VP9LfSync *lf_sync) {
#if CONFIG_MULTITHREAD
  assert(lf_sync != NULL);
  if (lf_sync->mutex != NULL) {
    ...
  }
#endif

  vpx_free(lf_sync->lfdata);
}
```

Good:

```cpp
void vp9_loop_filter_dealloc(VP9LfSync *lf_sync) {
  assert(lf_sync != NULL);

#if CONFIG_MULTITHREAD
  if (lf_sync->mutex != NULL) {
    ...
  }
#endif

  vpx_free(lf_sync->lfdata);
}
```
