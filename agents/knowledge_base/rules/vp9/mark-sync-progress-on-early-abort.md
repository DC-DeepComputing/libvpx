---
name: libvpx/vp9/mark-sync-progress-on-early-abort
rubric: safety
severity: should_fix
---

**Rule.** In multi-threaded tile or row decoding pipelines where worker threads
synchronize progress (such as loop filter synchronization via VP9LfSync and
vp9_set_row), abort early upon detecting bitstream corruption and mark all
remaining un-decoded rows across remaining tiles as completed in both early loop
breaks and error or longjmp handlers. Do not leave progress counters incomplete
on error paths, as waiting worker threads will block indefinitely.

**Why.** Worker threads synchronizing on row or tile progress spin or wait on
progress counters. If a worker terminates or skips processing due to corrupted
bitstreams or longjmp errors without updating its progress counters for
remaining rows and tiles, dependent threads will wait forever and deadlock.

## Examples

Bad:

```cpp
  do {
    for (mi_row = tile->mi_row_start; mi_row < tile->mi_row_end;
         mi_row += MI_BLOCK_SIZE) {
      for (mi_col = tile->mi_col_start;
           mi_col < tile->mi_col_end && !tile_data->xd.corrupted;
           mi_col += MI_BLOCK_SIZE) {
        decode_partition(tile_data, pbi, mi_row, mi_col, BLOCK_64X64, 4);
      }
      if (cm->lf.filter_level && !cm->skip_loop_filter) {
        vp9_set_row(lf_sync, 1 << cm->log2_tile_cols,
                    mi_row >> MI_BLOCK_SIZE_LOG2, is_last_row);
      }
    }
  } while (++n <= tile_data->buf_end);
```

Good:

```cpp
  do {
    for (mi_row = tile->mi_row_start; mi_row < tile->mi_row_end;
         mi_row += MI_BLOCK_SIZE) {
      for (mi_col = tile->mi_col_start;
           mi_col < tile->mi_col_end && !tile_data->xd.corrupted;
           mi_col += MI_BLOCK_SIZE) {
        decode_partition(tile_data, pbi, mi_row, mi_col, BLOCK_64X64, 4);
      }
      if (cm->lf.filter_level && !cm->skip_loop_filter) {
        vp9_set_row(lf_sync, 1 << cm->log2_tile_cols,
                    mi_row >> MI_BLOCK_SIZE_LOG2, is_last_row);
      }
    }
  } while (!tile_data->xd.corrupted && ++n <= tile_data->buf_end);

  // Unblock waiting worker threads for remaining un-decoded tiles/rows on error
  if (n <= tile_data->buf_end && cm->lf.filter_level && !cm->skip_loop_filter) {
    set_rows_after_error(lf_sync, cm->mi_rows, tile_data->buf_end - n,
                         1 << cm->log2_tile_cols);
  }
```
