---
name: libvpx/c-generic-pointer-helper-void-ptr-params
description: When writing generic C helper functions that mutate pointer variables by address, declare parameters as void * rather than void ** to eliminate explicit call-site casts
rubric: readability
severity: nit
---

**Rule.** In C helper functions that operate generically on pointer variables
passed by address (such as swapping or reassigning pointers), declare parameters
as `void *` rather than `void **` and cast to `void **` inside the helper body.
Avoid forcing call sites to explicitly cast arguments to `(void **)`.

**Why.** In C, any pointer type (including `T**`) implicitly converts to `void
*`, but `T**` does not implicitly convert to `void **`. Declaring generic
pointer-manipulating helper parameters as `void *` keeps call sites clean by
isolating the necessary cast inside the helper function.

## Examples

Bad:

```cpp
static void swap_ptr(void **a, void **b) {
  void *c = *a;
  *a = *b;
  *b = c;
}

// Call site:
swap_ptr((void **)&cr->map, (void **)&lc->map);
```

Good:

```cpp
static void swap_ptr(void *a, void *b) {
  void **a_p = (void **)a;
  void **b_p = (void **)b;
  void *c = *a_p;
  *a_p = *b_p;
  *b_p = c;
}

// Call site:
swap_ptr(&cr->map, &lc->map);
```
