---
name: libvpx/vp8/match-mv-and-boundary-limit-scales
description: When clamping or bounding motion vectors during motion search, verify coordinate resolutions match (full-pel vs. subpel) before applying limits
rubric: correctness
severity: should_fix
---

**Rule.** When clamping, bounding, or comparing motion vectors during motion
search, ensure the motion vector and boundary limits share the same coordinate
resolution (full-pel vs. 1/8-pel subpel). Do not pass subpel motion vectors to
full-pel clamping bounds (such as x->mv_col_min or x->mv_row_min) without
converting units (e.g., shifting by 3 or scaling by 8), and use dedicated subpel
clamping helpers (such as vp8_clamp_mv2 or vp9_set_subpel_mv_search_range) when
working with subpel coordinates.

**Why.** Motion vectors in VP8 and VP9 use 1/8-pel subpixel units for subpel
search and reference prediction, whereas macroblock motion limits are often
stored in full-pixel units. Clamping a subpel vector against full-pixel limits
introduces an 8x unit mismatch that severely distorts motion vectors and
prematurely clips valid motion search trajectories.

## Examples

Bad:

```cpp
// best_ref_mv is in 1/8-pel units, but mv_col_min/max are in full-pixel units:
vp8_clamp_mv(&best_ref_mv, x->mv_col_min, x->mv_col_max, x->mv_row_min,
             x->mv_row_max);
```

Good:

```cpp
// Convert to full-pel coordinates before clamping with full-pixel limits:
int_mv mvp_full;
mvp_full.as_mv.col = best_ref_mv.as_mv.col >> 3;
mvp_full.as_mv.row = best_ref_mv.as_mv.row >> 3;
vp8_clamp_mv(&mvp_full, x->mv_col_min, x->mv_col_max, x->mv_row_min,
             x->mv_row_max);
```
