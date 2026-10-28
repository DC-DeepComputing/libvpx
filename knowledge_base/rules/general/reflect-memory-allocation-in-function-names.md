---
name: libvpx/reflect-memory-allocation-in-function-names
description: When defining functions that allocate heap memory for caller data, include 'alloc' in the function name and provide matching free helpers
rubric: readability
severity: should_fix
---

**Rule.** When declaring or implementing functions that dynamically allocate
heap memory for caller output structures or buffers (rather than operating on
preallocated buffers), explicitly reflect memory allocation in the function name
(such as including an 'alloc' prefix or infix) and provide a matching free or
cleanup function.

**Why.** Naming allocating functions as pure read, query, or get operations
obscures internal dynamic memory allocation, leaving callers unaware of
ownership transfer and risking memory leaks. Explicit naming and paired cleanup
functions make memory lifecycle contracts transparent.

## Examples

Bad:

```cpp
// Conceals heap allocation in a 'read' function without a paired cleanup helper.
vpx_codec_err_t vpx_read_tpl_stats(FILE *tpl_file,
                                   VpxTplGopStats *tpl_gop_stats);
```

Good:

```cpp
// Explicitly indicates allocation and provides a paired free helper.
vpx_codec_err_t vpx_alloc_read_tpl_stats(FILE *tpl_file,
                                         VpxTplGopStats *tpl_gop_stats);
void vpx_free_tpl_gop_stats(VpxTplGopStats *tpl_gop_stats);
```
