---
name: libvpx/vp9/no-mode-info-checks-prior-to-rdo
description: When implementing partition pruning or block splitting heuristics prior to rate-distortion mode optimization, do not read mode info fields from xd->mi[0]
rubric: correctness
severity: should_fix
---

**Rule.** Do not inspect block mode decision fields from `xd->mi[0]` (such as
`ref_frame`, `mode`, `uv_mode`, or `interp_filter`) during partition search or
partition pruning prior to rate-distortion mode optimization
(`rd_pick_sb_modes`). At the partition selection stage, mode selection for the
current block has not yet occurred, and `xd->mi[0]` holds uninitialized or stale
data from previous block iterations.

**Why.** In VP9 recursive partition search (such as `rd_pick_partition`),
partition pruning decisions are evaluated before testing and picking modes for
the candidate block. Querying `xd->mi[0]` mode fields at this stage reads stale
mode info from previous macroblocks or earlier partition evaluations rather than
valid decisions for the current block, leading to incorrect pruning heuristics
and unpredictable encoding behavior.

## Examples

Bad:

```cpp
const int do_rd_ml_partition_var_pruning =
    partition_none_allowed && do_split &&
    mi_row + num_8x8_blocks_high_lookup[bsize] <= cm->mi_rows &&
    mi_col + num_8x8_blocks_wide_lookup[bsize] <= cm->mi_cols &&
    xd->mi[0]->ref_frame[0] != INTRA_FRAME;
```

Good:

```cpp
const int do_rd_ml_partition_var_pruning =
    partition_none_allowed && do_split &&
    mi_row + num_8x8_blocks_high_lookup[bsize] <= cm->mi_rows &&
    mi_col + num_8x8_blocks_wide_lookup[bsize] <= cm->mi_cols;
```
