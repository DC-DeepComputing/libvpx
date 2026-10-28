---
name: libvpx/use-vpx-mem-allocation-functions
description: When allocating or freeing dynamic memory inside the library, use vpx_malloc, vpx_calloc, and vpx_free instead of standard malloc, calloc, and free
rubric: readability
severity: should_fix
---

**Rule.** Use `vpx_malloc`, `vpx_calloc`, `vpx_free`, and `vpx_memalign` (from
`vpx_mem/vpx_mem.h`) instead of standard C library functions (`malloc`,
`calloc`, `free`, `realloc`, `memalign`) for internal dynamic memory management
within the library.

**Why.** `vpx_mem` wrapper functions provide platform-independent memory
alignment, support internal memory tracking and debug accounting, and ensure
consistent allocator usage across core library modules.

## Examples

Bad:

```cpp
tpl_gop_stats->frame_stats_list = (VpxTplFrameStats *)calloc(
    frame_list_size, sizeof(tpl_gop_stats->frame_stats_list[0]));

/* ... */
free(data->frame_stats_list);
```

Good:

```cpp
#include "vpx_mem/vpx_mem.h"

tpl_gop_stats->frame_stats_list = (VpxTplFrameStats *)vpx_calloc(
    frame_list_size, sizeof(tpl_gop_stats->frame_stats_list[0]));

/* ... */
vpx_free(data->frame_stats_list);
```
