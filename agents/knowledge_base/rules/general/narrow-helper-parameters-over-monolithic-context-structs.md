---
name: libvpx/narrow-helper-parameters-over-monolithic-context-structs
description: When defining helper, utility, or computation functions, pass only specific required sub-structures or pointers instead of monolithic context structs like VP9_COMP, VP8_COMP, or MACROBLOCKD
rubric: design
severity: should_fix
---

**Rule.** When declaring or implementing helper functions, utilities, or
internal calculation routines, pass only the specific sub-structures, data
buffers, function pointer tables, or state pointers required (such as struct
vpx_internal_error_info *, VpxTplGopStats *, TplDepFrame *, YV12_BUFFER_CONFIG
*, or vp9_variance_fn_ptr_t *) instead of passing monolithic top-level context
structs like VP9_COMP *, VP8_COMP *, VP9_COMMON *, VP8_COMMON *, MACROBLOCK *,
or MACROBLOCKD *.

**Why.** Passing large monolithic context structs into helper functions obscures
which data is read or mutated, tightly couples helper logic to global codec
state and internal header definitions, and bloats the effective scope of the
function. Narrowing parameters to only the necessary sub-structures minimizes
coupling, clarifies data dependencies, and makes helper functions easier to
understand, unit test, and reuse across modules.

## Examples

Bad:

```cpp
// Bad: Helper accepts full encoder or macroblock context when only error info,
// TPL stats, or specific buffers and variance function pointers are needed.
static void init_tpl_stats_before_propagation(VP9_COMP *cpi,
                                              int tpl_gop_frames) {
  VpxTplGopStats *const tpl_gop_stats = &cpi->tpl_gop_stats;
  TplDepFrame *const tpl_stats = cpi->tpl_stats;
  // ...
}

void get_local_structure(VP9_COMP *cpi, MACROBLOCKD *xd,
                         YV12_BUFFER_CONFIG *frame[3], BLOCK_SIZE bsize,
                         int *M[3]);
```

Good:

```cpp
// Good: Pass only the specific sub-structures, buffers, and error context required.
static void init_tpl_stats_before_propagation(
    struct vpx_internal_error_info *error_info, VpxTplGopStats *tpl_gop_stats,
    TplDepFrame *tpl_stats, int tpl_gop_frames) {
  // ...
}

void vp9_get_local_structure(const YV12_BUFFER_CONFIG *ref_frame,
                             const vp9_variance_fn_ptr_t *fn_ptr, int mi_rows,
                             int mi_cols, BLOCK_SIZE bsize, int (*M)[4]);
```
