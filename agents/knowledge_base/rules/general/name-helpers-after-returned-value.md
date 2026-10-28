---
name: libvpx/name-helpers-after-returned-value
description: When defining helper functions that calculate values or buffer sizes, name them after the specific returned entity, metric, or paired caller routine
rubric: readability
severity: should_fix
---

**Rule.** Name calculation, sizing, and value-returning helper functions after
the specific metric, entity, or data unit they calculate (e.g., `..._segment`
rather than `..._rdmult` when computing segment IDs, or
`encode_tiles_buffer_alloc_size` rather than `frame_alloc_size` for tile
encoding buffers). Ensure the helper name accurately matches the returned value,
actual data unit scope, and corresponding caller allocation or lifecycle
routines rather than using downstream variables or overly broad entity names.

**Why.** Inaccurate or overly broad helper names (such as naming a function
after downstream variables or using 'frame' for tile-level buffers) obscure the
function's true return value and scope, misleading callers and causing confusion
during maintenance. Precise naming clarifies the function contract and keeps
caller and callee routines consistent.

## Examples

Bad:

```cpp
// Anti-pattern 1: Named after downstream variable rather than returned entity
static int wiener_var_rdmult(VP9_COMP *cpi, BLOCK_SIZE bsize, int mi_row,
                             int mi_col) {
  int segment_id = ...;
  return segment_id;
}

// Anti-pattern 2: Overly broad entity name mismatched with caller routine
static size_t frame_alloc_size(const VP9_COMP *cpi) {
  return cpi->Source->y_crop_width * cpi->Source->y_crop_height +
         2 * cpi->Source->uv_crop_width * cpi->Source->uv_crop_height;
}
```

Good:

```cpp
// Pattern 1: Named after computed and returned entity
static int wiener_var_segment(VP9_COMP *cpi, BLOCK_SIZE bsize, int mi_row,
                              int mi_col) {
  int segment_id = ...;
  return segment_id;
}

// Pattern 2: Specific entity and matching caller allocation routine
static size_t encode_tiles_buffer_alloc_size(const VP9_COMP *cpi) {
  return cpi->Source->y_crop_width * cpi->Source->y_crop_height +
         2 * cpi->Source->uv_crop_width * cpi->Source->uv_crop_height;
}
```
