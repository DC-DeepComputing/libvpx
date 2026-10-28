---
name: libvpx/encapsulate-buffer-reallocation-in-helpers
description: When allocating or reinitializing dynamic buffers, encapsulate capacity checks, buffer reuse, and old or dependent resource deallocation inside allocation helpers rather than caller sites
rubric: design
severity: should_fix
---

**Rule.** When allocating, resizing, or reinitializing dynamic buffers and
context structures (such as tile data, multi-threading worker structures, or
motion field info), encapsulate capacity validation, buffer reuse, and
deallocation of existing or dependent sub-resources inside the allocation helper
function itself. Inside the helper, check whether the buffer is already
allocated: early-return if existing capacity is sufficient, or release existing
parent memory and dependent sub-allocations before allocating new memory.
Callers must invoke allocation helpers directly without duplicating buffer null
checks, dimension threshold comparisons, or manual cleanup routines.

**Why.** Centralizing capacity checks, buffer reuse, and prior resource
deallocation inside allocation helpers prevents memory leaks across frame
resizing and resolution changes, eliminates redundant pre-checks and cleanup
logic across caller loops, and guarantees consistent allocation lifecycle
management across all call sites.

## Examples

Bad:

```cpp
// Anti-pattern 1: Caller loop checks capacity and nullness before calling helper
for (tile_row = 0; tile_row < tile_rows; ++tile_row) {
  for (tile_col = 0; tile_col < tile_cols; ++tile_col) {
    TileDataEnc *this_tile = &cpi->tile_data[tile_row * tile_cols + tile_col];
    if (this_tile->row_base_thresh_freq_fact == NULL || sb_rows > this_tile->sb_rows) {
      vp9_row_mt_alloc_rd_thresh(cpi, this_tile);
    }
  }
}

// Anti-pattern 2: Caller manually checks and frees dependent resources before reinit
if (cpi->tile_data != NULL && cpi->allocated_tiles < tile_cols * tile_rows) {
  vp9_row_mt_mem_dealloc(cpi);
}
vp9_init_tile_data(cpi);

// Anti-pattern 3: Helper allocates without releasing existing memory on resize
Status vp9_alloc_motion_field_info(MotionFieldInfo *info, int frame_num) {
  info->motion_field_array = vpx_calloc(frame_num, sizeof(*info->motion_field_array));
  if (!info->motion_field_array) return STATUS_ERROR;
  info->allocated = 1;
  return STATUS_OK;
}
```

Good:

```cpp
// Pattern 1 & 2: Callers directly invoke allocation helpers without pre-checks
for (tile_row = 0; tile_row < tile_rows; ++tile_row) {
  for (tile_col = 0; tile_col < tile_cols; ++tile_col) {
    TileDataEnc *this_tile = &cpi->tile_data[tile_row * tile_cols + tile_col];
    vp9_row_mt_alloc_rd_thresh(cpi, this_tile);
  }
}
vp9_init_tile_data(cpi);

// Pattern 3: Allocation helpers encapsulate reuse checks and prior/dependent deallocation
void vp9_row_mt_alloc_rd_thresh(VP9_COMP *const cpi, TileDataEnc *const this_tile) {
  if (this_tile->row_base_thresh_freq_fact != NULL) {
    if (sb_rows <= this_tile->sb_rows) return;
    vpx_free(this_tile->row_base_thresh_freq_fact);
    this_tile->row_base_thresh_freq_fact = NULL;
  }
  CHECK_MEM_ERROR(&cpi->common, this_tile->row_base_thresh_freq_fact,
                  vpx_calloc(sb_rows * 8, sizeof(*this_tile->row_base_thresh_freq_fact)));
  this_tile->sb_rows = sb_rows;
}

Status vp9_alloc_motion_field_info(MotionFieldInfo *info, int frame_num) {
  if (info->allocated == 1) vp9_free_motion_field_info(info);
  info->motion_field_array = vpx_calloc(frame_num, sizeof(*info->motion_field_array));
  if (!info->motion_field_array) return STATUS_ERROR;
  info->allocated = 1;
  return STATUS_OK;
}
```
