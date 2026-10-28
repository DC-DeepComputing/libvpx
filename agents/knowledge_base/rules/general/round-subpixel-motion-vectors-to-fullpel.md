---
name: libvpx/round-subpixel-motion-vectors-to-fullpel
description: When converting signed sub-pixel motion vectors or fixed-point coordinates to full-pixel integers, apply signed symmetric rounding instead of truncating with integer division
rubric: correctness
severity: should_fix
---

**Rule.** When converting fractional or sub-pixel motion vectors (such as
1/8-pel or 1/4-pel coordinates) and signed fixed-point displacements to
full-pixel integer values, compute symmetric rounding to the nearest integer
(e.g., `(val >= 0 ? 1 : -1) * (abs(val) + half_step) / step`). Do not truncate
signed sub-pixel coordinates toward zero using plain integer division or
arithmetic right shifts without rounding offset.

**Why.** Integer division truncates signed values toward zero and bare right
shifts truncate toward negative infinity, introducing directional bias and
systematic coordinate drift. Symmetric signed rounding preserves spatial
fidelity by mapping fractional motion vector displacements symmetrically to
their closest full-pixel grid locations.

## Examples

Bad:

```cpp
// Truncates toward zero, losing precision and introducing directional bias
tpl_block_stats_ptr->mv_r = src_stats->mv.as_mv.row / 8;
tpl_block_stats_ptr->mv_c = src_stats->mv.as_mv.col / 8;
```

Good:

```cpp
// Round symmetrically to nearest full-pixel integer
tpl_block_stats_ptr->mv_r = (src_stats->mv.as_mv.row >= 0 ? 1 : -1) *
                            (abs(src_stats->mv.as_mv.row) + 4) / 8;
tpl_block_stats_ptr->mv_c = (src_stats->mv.as_mv.col >= 0 ? 1 : -1) *
                            (abs(src_stats->mv.as_mv.col) + 4) / 8;
```
