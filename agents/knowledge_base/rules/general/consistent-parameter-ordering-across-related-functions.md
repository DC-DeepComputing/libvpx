---
name: libvpx/consistent-parameter-ordering-across-related-functions
description: When defining related lifecycle or helper functions operating on common structures, maintain consistent parameter ordering with target struct pointers preceding context structs
rubric: readability
severity: should_fix
---

**Rule.** Maintain consistent parameter ordering across related functions and
lifecycle operations (such as allocation, initialization, execution, and
cleanup) that operate on the same data structures. Position the target data
structure pointer (the receiver struct) as the first parameter, followed by
context structs (such as codec or common state) and scalar configuration
parameters.

**Why.** Inconsistent argument ordering across companion functions creates
confusion, obscures the primary receiver of the operation, and increases the
risk of subtle caller-side parameter transposition bugs when invoking related
lifecycle routines.

## Examples

Bad:

```cpp
// Inconsistent: target sync struct is placed second in init but first in alloc/dealloc
void vp9_loop_filter_alloc(VP9LfSync *lf_sync, VP9_COMMON *cm, int rows,
                           int num_workers);
void vp9_lpf_mt_init(VP9_COMMON *cm, VP9LfSync *lf_sync,
                     int frame_filter_level, int num_workers);
```

Good:

```cpp
// Consistent: target sync struct is placed first across all related functions
void vp9_loop_filter_alloc(VP9LfSync *lf_sync, VP9_COMMON *cm, int rows,
                           int num_workers);
void vp9_lpf_mt_init(VP9LfSync *lf_sync, VP9_COMMON *cm,
                     int frame_filter_level, int num_workers);
```
