---
name: libvpx/avoid-redundant-buffer-size-temporaries
description: When allocating and initializing memory buffers with vpx_malloc or memset, pass simple size expressions inline rather than declaring redundant temporary size variables
rubric: readability
severity: nit
---

**Rule.** Do not declare redundant single-use temporary variables solely to hold
buffer sizes when allocating, reallocating, or initializing memory buffers. Pass
simple sizing expressions inline directly to vpx_malloc, vpx_calloc, and memset,
reserving dedicated size variables for complex multi-step computations or values
shared across distinct operations.

**Why.** Declaring one-off temporary size variables for straightforward buffer
allocations and memset calls clutters the local function scope with unnecessary
identifiers. Inlining simple sizing expressions keeps allocation and
initialization concise and easy to follow.

## Examples

Bad:

```cpp
size_t last_coded_q_map_size =
    cm->mi_rows * cm->mi_cols * sizeof(*lc->last_coded_q_map);
vpx_free(lc->last_coded_q_map);
CHECK_MEM_ERROR(&cm->error, lc->last_coded_q_map,
                vpx_malloc(last_coded_q_map_size));
memset(lc->last_coded_q_map, MAXQ, last_coded_q_map_size);
```

Good:

```cpp
vpx_free(lc->last_coded_q_map);
CHECK_MEM_ERROR(&cm->error, lc->last_coded_q_map,
                vpx_malloc(cm->mi_rows * cm->mi_cols *
                           sizeof(*lc->last_coded_q_map)));
memset(lc->last_coded_q_map, MAXQ,
       cm->mi_rows * cm->mi_cols * sizeof(*lc->last_coded_q_map));
```
