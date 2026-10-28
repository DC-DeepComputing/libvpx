---
name: libvpx/pair-resource-init-and-cleanup-helpers
description: When defining dedicated initialization or allocation helpers for struct members, create matching deallocation helpers and colocate their definitions
rubric: design
severity: should_fix
---

**Rule.** When defining a dedicated initialization or allocation helper for a
struct member or subsystem component (e.g., *_init or *_alloc), create a
matching deallocation helper (e.g., free_* or *_destroy) and colocate their
definitions in the same header or source file. Call the deallocation helper from
top-level cleanup routines rather than inlining raw vpx_free() calls and manual
NULL assignments.

**Why.** Encapsulating allocation and deallocation in symmetrically paired
helpers keeps resource lifecycle management modular and cohesive. It avoids
scattering raw deallocation logic across top-level cleanup routines and ensures
that state resets (such as nulling freed pointers) remain synchronized with
initialization.

## Examples

Bad:

```cpp
// In dealloc_compressor_data() in vp9_encoder.c:
#if CONFIG_RATE_CTRL
  vpx_free(cpi->partition_info);
  cpi->partition_info = NULL;
#endif
```

Good:

```cpp
// In vp9_partition_models.h (colocated next to partition_info_init):
static INLINE void free_partition_info(struct VP9_COMP *cpi) {
  vpx_free(cpi->partition_info);
  cpi->partition_info = NULL;
}

// In dealloc_compressor_data() in vp9_encoder.c:
#if CONFIG_RATE_CTRL
  free_partition_info(cpi);
#endif
```
