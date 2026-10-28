---
name: libvpx/avoid-redundant-allocation-flags
description: When tracking dynamic memory allocation or initialization status, check the pointer directly against NULL instead of adding redundant boolean state flags
rubric: readability
severity: should_fix
---

**Rule.** Do not introduce separate boolean flags (such as mem_alloc or
is_allocated) to track dynamic buffer allocation or initialization status. Check
the allocated pointer directly against NULL (e.g., if (ptr != NULL) or if (ptr))
to determine whether memory has been allocated.

**Why.** Tracking allocation status with a separate boolean flag duplicates
state already represented by pointer nullness. Checking the pointer directly
simplifies struct definitions, prevents flag-pointer desynchronization bugs, and
eliminates redundant state bookkeeping.

## Examples

Bad:

```cpp
struct FrameData {
  uint8_t mem_alloc;
  StatsBuffer *stats_ptr;
};

if (frame_data->mem_alloc) return;
frame_data->stats_ptr = vpx_calloc(1, sizeof(*frame_data->stats_ptr));
frame_data->mem_alloc = 1;
```

Good:

```cpp
struct FrameData {
  StatsBuffer *stats_ptr;
};

if (frame_data->stats_ptr != NULL) return;
frame_data->stats_ptr = vpx_calloc(1, sizeof(*frame_data->stats_ptr));
```
