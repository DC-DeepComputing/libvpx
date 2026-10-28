---
name: libvpx/vp9/reuse-svc-per-layer-dimension-arrays
description: In VP9 spatial SVC, retrieve lower layer dimensions from stored svc->mi_cols/svc->mi_rows arrays instead of halving current dimensions, querying cm->prev_frame, or adding redundant struct fields
rubric: correctness
severity: blocking
---

**Rule.** In VP9 spatial SVC or multi-resolution encoding, retrieve lower
spatial layer block grid dimensions (such as mi_cols, mi_rows, and mi_stride)
from the existing per-layer context arrays (cpi->svc.mi_cols[prev_layer],
cpi->svc.mi_rows[prev_layer], and cpi->svc.mi_stride[prev_layer]). Do not derive
lower layer dimensions by halving or bit-shifting current layer dimensions
(e.g., (cm->mi_cols + 1) >> 1), do not query cm->prev_frame dimensions directly,
and do not add redundant struct fields to track previous layer dimensions.

**Why.** Frame dimensions are aligned to MI block sizes before calculating
mi_cols and mi_rows. For odd or non-aligned resolutions, halving the upper
layer's aligned block count does not match the lower layer's actual aligned
block count, causing index calculation errors, buffer overflow, and memory
corruption. Furthermore, struct SVC already tracks macroblock dimensions for all
spatial layers; adding redundant dimension fields creates duplicate state prone
to desynchronization, while frame buffer dimensions on cm->prev_frame are not
guaranteed to match the logical dimensions of the previous spatial layer.

## Examples

Bad:

```cpp
// Bad: Deriving lower spatial layer dimensions by halving current frame dimensions
int prev_frame_mi_cols = (cm->mi_cols + 1) >> 1;
int prev_frame_mi_rows = (cm->mi_rows + 1) >> 1;
int index = (mi_col >> 1) + (mi_row >> 1) * prev_frame_mi_cols;

// Bad: Adding redundant dimension fields to struct SVC
typedef struct SVC {
  int prev_frame_mi_cols;
  int prev_frame_mi_rows;
} SVC;
```

Good:

```cpp
// Good: Reusing existing per-layer dimension arrays from struct SVC
const int prev_layer = cpi->svc.spatial_layer_id - 1;
const int prev_mi_cols = cpi->svc.mi_cols[prev_layer];
const int prev_mi_rows = cpi->svc.mi_rows[prev_layer];
const int index = (mi_col >> 1) + (mi_row >> 1) * prev_mi_cols;
if (index < prev_mi_cols * prev_mi_rows) {
  // Safely index lower spatial layer data
}
```
