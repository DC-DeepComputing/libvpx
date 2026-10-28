---
name: libvpx/multiply-element-count-by-sizeof-for-buffers
description: When sizing buffer operations (memset, memcpy, memmove) or allocations (vpx_calloc, malloc), use sizeof(*ptr) or sizeof(var) instead of literal byte counts, unscaled counts, sizeof(Type), or sizeof(ptr[0])
rubric: readability
severity: should_fix
---

**Rule.** When calculating buffer byte sizes or allocation element sizes for
memory functions (such as vpx_calloc, vpx_malloc, vpx_realloc, vpx_memalign,
malloc, calloc, memset, memcpy, memmove, or memcmp):
1. Always derive buffer byte counts using sizeof(...) rather than hardcoding numeric byte counts, array lengths, or magic numbers.
2. When allocating arrays or operating on multi-byte elements, always multiply element counts by the element size.
3. Size elements using dereferenced pointer syntax sizeof(*ptr) (or sizeof(var) / sizeof(array) for fixed buffers) rather than explicit type names (such as sizeof(Type)) or array indexing syntax sizeof(ptr[0]).

**Why.** Hardcoding literal byte counts or omitting the element size multiplier
risks memory corruption, heap under-allocation, or partial buffer
copying/clearing whenever struct definitions, array dimensions, or element types
change. Deriving sizes directly with sizeof(*ptr) or sizeof(var) binds the byte
computation directly to the destination or source variable, keeping sizing
expressions uniform across the codebase and preventing silent type/size
mismatches during refactoring.

## Examples

Bad:

```cpp
// Anti-pattern 1: Hardcoded numeric byte count in memory operations.
memcpy(pbi->common.error.detail, pbi->mb.error_info.detail, 80);
memset(ctx->probs, 0, 128);

// Anti-pattern 2: Omitting sizeof assumes 1-byte elements.
memset(cr->last_coded_q_map, MAXQ, cm->mi_rows * cm->mi_cols);

// Anti-pattern 3: Sizing by explicit type name instead of target pointer.
tpl_gop_stats->frame_stats_list =
    (VpxTplFrameStats *)calloc(frame_list_size, sizeof(VpxTplFrameStats));

// Anti-pattern 4: Using sizeof(ptr[0]) instead of standard sizeof(*ptr).
CHECK_MEM_ERROR(
    cm, cpi->skin_map,
    vpx_calloc(cm->mi_rows * cm->mi_cols, sizeof(cpi->skin_map[0])));
```

Good:

```cpp
// Derive byte count using sizeof on the target buffer or struct.
memcpy(pbi->common.error.detail, pbi->mb.error_info.detail,
       sizeof(pbi->mb.error_info.detail));
memset(ctx->probs, 0, sizeof(ctx->probs));

// Explicitly scale byte count with sizeof(*ptr).
memset(cr->last_coded_q_map, MAXQ,
       cm->mi_rows * cm->mi_cols * sizeof(*cr->last_coded_q_map));

// Sizing with sizeof(*ptr) couples size directly to destination variable.
tpl_gop_stats->frame_stats_list = (VpxTplFrameStats *)calloc(
    frame_list_size, sizeof(*tpl_gop_stats->frame_stats_list));

// Consistently use standard sizeof(*ptr) syntax.
CHECK_MEM_ERROR(
    cm, cpi->skin_map,
    vpx_calloc(cm->mi_rows * cm->mi_cols, sizeof(*cpi->skin_map)));
```
