---
name: libvpx/vp9/derive-block-dimensions-from-block-size-lookups
description: When performing block operations or iterating grids in VP9, derive dimensions, strides, and step sizes from BLOCK_SIZE lookup tables
rubric: readability
severity: should_fix
---

**Rule.** Avoid hardcoding numeric block dimensions, strides, pixel counts, or
step sizes directly into VP9 block processing routines and mode info iteration
loops. Derive pixel dimensions and offsets from the active BLOCK_SIZE enum using
lookup tables such as b_width_log2_lookup and b_height_log2_lookup. When
iterating across mode info grids or computing block steps, derive step
increments from BLOCK_SIZE using standard lookup tables (e.g.,
num_8x8_blocks_high_lookup, num_8x8_blocks_wide_lookup, num_4x4_blocks_*,
mi_width_log2_lookup) or named constants like MI_BLOCK_SIZE.

**Why.** Hardcoded numeric literals and step sizes obscure block geometry and
unit scale (such as pixels vs. 8x8 mode info units vs. 4x4 sub-blocks). Using
standard BLOCK_SIZE lookup tables ensures consistency across prediction,
subtraction, transform, and motion estimation routines, and keeps loops
resilient to block configuration changes and refactoring.

## Examples

Bad:

```cpp
// Hardcoded block dimensions passed to predictor and subtraction routines
vp9_highbd_build_inter_predictor(
    ref_buf, ref_stride, predictor, 32, &mv.as_mv, &sf, 32, 32, 0, kernel,
    MV_PRECISION_Q3, mi_col * MI_SIZE, mi_row * MI_SIZE, xd->bd);
vpx_highbd_subtract_block(32, 32, src_diff, 32, src, src_stride, predictor, 16,
                          xd->bd);

// Hardcoded step size when iterating mode info grid
for (mi_row = 0; mi_row < cm->mi_rows; mi_row += 4) {
  for (mi_col = 0; mi_col < cm->mi_cols; mi_col += 4) {
    // Process block...
  }
}
```

Good:

```cpp
const BLOCK_SIZE bsize = BLOCK_32X32;
const int bw = 4 << b_width_log2_lookup[bsize];
const int bh = 4 << b_height_log2_lookup[bsize];

// Dimensions derived from block size lookups
vp9_highbd_build_inter_predictor(
    ref_buf, ref_stride, predictor, bw, &mv.as_mv, &sf, bw, bh, 0, kernel,
    MV_PRECISION_Q3, mi_col * MI_SIZE, mi_row * MI_SIZE, xd->bd);
vpx_highbd_subtract_block(bh, bw, src_diff, bw, src, src_stride, predictor, bw,
                          xd->bd);

// Mode info step sizes derived from block size lookups
const int mi_height = num_8x8_blocks_high_lookup[bsize];
const int mi_width = num_8x8_blocks_wide_lookup[bsize];
for (mi_row = 0; mi_row < cm->mi_rows; mi_row += mi_height) {
  for (mi_col = 0; mi_col < cm->mi_cols; mi_col += mi_width) {
    // Process block...
  }
}
```
