---
name: libvpx/document-non-obvious-buffer-sizing-invariants
description: When performing non-obvious buffer indexing or boundary checks across differing layer or frame dimensions, document sizing invariants with explanatory comments
rubric: readability
severity: nit
---

**Rule.** When performing buffer indexing or bounds checks on arrays whose
dimensions or allocation sizes differ from the current frame or block context
(such as cross-layer motion vector buffers or scaled reference frames), document
the sizing invariants and boundary logic with an explanatory comment. State
clearly which layer, frame, or geometry context determines the buffer size and
why the boundary condition is necessary.

**Why.** Buffers sized for external contexts (such as previous spatial layers in
SVC or scaled reference frames) do not follow the current frame's dimensions.
Without explanatory comments detailing the allocation invariant, non-obvious
index arithmetic and boundary guards are difficult to understand, verify, or
review, often leading to subtle out-of-bounds access or incorrect boundary
checks during refactoring.

## Examples

Bad:

```cpp
// Non-obvious index and boundary check against current frame dimensions
// when accessing motion vectors allocated for a previous spatial layer.
if (mi_col < cm->mi_cols - (cm->mi_cols % 2) &&
    mi_row < cm->mi_rows - (cm->mi_rows % 2)) {
  MV_REF *candidate =
      &cm->prev_frame
           ->mvs[(mi_col >> 1) + (mi_row >> 1) * (cm->mi_cols >> 1)];
}
```

Good:

```cpp
const int prev_layer = cpi->svc.spatial_layer_id - 1;
const int index =
    (mi_col >> 1) + (mi_row >> 1) * cpi->svc.mi_cols[prev_layer];
// prev_frame->mvs[] is allocated to size mi_cols * mi_rows corresponding
// to the previous spatial layer, so the index check is against
// svc.mi_cols/rows[prev_layer].
if (index < cpi->svc.mi_cols[prev_layer] * cpi->svc.mi_rows[prev_layer]) {
  MV_REF *candidate = &cm->prev_frame->mvs[index];
}
```
