---
name: libvpx/omit-redundant-null-check-before-vpx-free
description: When freeing memory with vpx_free() or free(), omit redundant null checks like if (ptr != NULL) because deallocating NULL is safe
rubric: readability
severity: nit
---

**Rule.** Do not guard calls to vpx_free() or standard free() with redundant
null pointer checks (such as if (ptr != NULL), if (ptr != nullptr), or if
(ptr)). Call vpx_free(ptr) or free(ptr) directly because both functions tolerate
NULL and nullptr as a safe no-op.

**Why.** vpx_free() follows standard C free() semantics and safely performs a
no-op on null pointers. Adding explicit null checks before deallocating memory
introduces redundant boilerplate and visual clutter without providing any safety
benefit.

## Examples

Bad:

```cpp
if (cpi->tpl_stats[frame].tpl_stats_ptr != NULL) {
  vpx_free(cpi->tpl_stats[frame].tpl_stats_ptr);
}

if (cpi_ != nullptr) {
  vpx_free(cpi_);
}
if (buffer != NULL) free(buffer);
```

Good:

```cpp
vpx_free(cpi->tpl_stats[frame].tpl_stats_ptr);
vpx_free(cpi_);
free(buffer);
```
