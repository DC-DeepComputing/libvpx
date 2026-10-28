---
name: libvpx/advance-row-pointer-by-stride-in-2d-traversal
description: When traversing 2D image, block, or transform buffers across rows and columns, advance pointers incrementally by stride per row instead of recalculating 2D index offsets
rubric: correctness
severity: should_fix
---

**Rule.** When iterating over 2D image buffers, blocks, frames, planes, or
transform arrays, initialize base pointers and advance them incrementally by the
buffer stride at each row step (e.g., `ptr += stride` per outer row iteration or
maintaining a running row pointer for inner row loops), indexing columns
directly (`ptr[col]`) or incrementing sequentially (`++ptr`). Avoid
recalculating linear buffer offsets such as `ptr[row * stride + col]` or
`&buf[(row + y) * stride + (col + x)]` inside inner loops.

**Why.** Repeatedly evaluating 2D index arithmetic with multiplications inside
tight inner loops introduces avoidable computational overhead and risks integer
arithmetic overflow on large dimensions or strides. Incrementing row pointers
incrementally by stride avoids redundant multiplications, simplifies address
generation across compilers, and aligns C reference implementations with SIMD
and assembly buffer traversal patterns.

## Examples

Bad:

```cpp
// Variant 1: Recalculating 2D offsets inside row/col nested loops
for (int idy = 0; idy < mi_height; ++idy) {
  for (int idx = 0; idx < mi_width; ++idx) {
    TplDepStats *tpl_ptr =
        &tpl_stats[(mi_row + idy) * stride + (mi_col + idx)];
    tpl_ptr->intra_cost = intra_cost;
  }
}

// Variant 2: Recomputing row index multiplication in column-outer loop
for (int i = 0; i < 16; ++i) {
  for (int j = 0; j < 16; ++j) {
    dest[j * stride + i] = highbd_clip_pixel_add(
        dest[j * stride + i], ROUND_POWER_OF_TWO(temp_out[j], 6), bd);
  }
}
```

Good:

```cpp
// Variant 1: Advance row pointer by stride per row, indexing columns directly
TplDepStats *tpl_ptr = &tpl_stats[mi_row * stride + mi_col];
for (int idy = 0; idy < mi_height; ++idy) {
  for (int idx = 0; idx < mi_width; ++idx) {
    tpl_ptr[idx].intra_cost = intra_cost;
  }
  tpl_ptr += stride;
}

// Variant 2: Initialize running row pointer and advance by stride per inner step
for (int i = 0; i < 16; ++i) {
  uint16_t *dest_row = dest;
  for (int j = 0; j < 16; ++j) {
    dest_row[i] = highbd_clip_pixel_add(
        dest_row[i], ROUND_POWER_OF_TWO(temp_out[j], 6), bd);
    dest_row += stride;
  }
}
```
