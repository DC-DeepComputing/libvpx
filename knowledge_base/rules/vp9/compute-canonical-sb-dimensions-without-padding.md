---
name: libvpx/vp9/compute-canonical-sb-dimensions-without-padding
description: When computing superblock dimension variables such as sb_rows or sb_cols, use canonical alignment formulas without adding padding offsets like + 1
rubric: correctness
severity: should_fix
---

**Rule.** Calculate standard frame and tile dimension variables (such as
superblock rows and columns, sb_rows and sb_cols) using canonical alignment
formulas (such as mi_cols_aligned_to_sb(cm->mi_rows) >> MI_BLOCK_SIZE_LOG2)
without adding padding offsets like + 1. If extra elements or padding rows are
needed for buffer storage, apply the offset directly to the allocation size
expression rather than redefining the dimension variable.

**Why.** Adding padding offsets directly to dimension variables creates
off-by-one discrepancies against standard dimension calculations across the
codebase. Storing padded values into struct fields (such as this_tile->sb_rows)
corrupts subsequent reallocation checks, loop bounds, and multi-threading job
distributions.

## Examples

Bad:

```cpp
// Bad: Adding + 1 to sb_rows corrupts stored struct dimensions and reallocation guards.
const int sb_rows =
    (mi_cols_aligned_to_sb(cm->mi_rows) >> MI_BLOCK_SIZE_LOG2) + 1;
CHECK_MEM_ERROR(
    &cm->error, this_tile->row_base_thresh_freq_fact,
    (int *)vpx_calloc(sb_rows * BLOCK_SIZES * MAX_MODES,
                      sizeof(*(this_tile->row_base_thresh_freq_fact))));
this_tile->sb_rows = sb_rows;
```

Good:

```cpp
// Good: Compute sb_rows canonically and store the exact superblock count.
const int sb_rows = mi_cols_aligned_to_sb(cm->mi_rows) >> MI_BLOCK_SIZE_LOG2;
CHECK_MEM_ERROR(
    &cm->error, this_tile->row_base_thresh_freq_fact,
    (int *)vpx_calloc(sb_rows * BLOCK_SIZES * MAX_MODES,
                      sizeof(*(this_tile->row_base_thresh_freq_fact))));
this_tile->sb_rows = sb_rows;
```
