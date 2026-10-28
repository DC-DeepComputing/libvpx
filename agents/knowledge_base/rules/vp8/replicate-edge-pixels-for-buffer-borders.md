---
name: libvpx/vp8/replicate-edge-pixels-for-buffer-borders
description: When extending buffer borders or copying partial frames above the top edge, replicate boundary rows stride-by-stride instead of zeroing memory or indexing negative offsets
rubric: correctness
severity: should_fix
---

**Rule.** When initializing, padding, extending buffer borders, or copying
partial frame slices for filtering or reference frame operations, replicate
adjacent edge pixels (e.g., repeating the boundary row or using frame border
extension helpers) rather than zeroing memory with memset. Additionally, when
handling negative row offsets (yoffset < 0) above the frame top, replicate the
first valid top row (top_row / y_buffer) stride-by-stride into each border row
above it instead of attempting a flat memcpy or indexing directly from negative
pointer offsets (y_buffer + yoffset).

**Why.** Frame buffers use row strides (ystride) that separate consecutive lines
in memory. Reading before the buffer start (y_buffer + yoffset) causes
out-of-bounds reads into uninitialized memory, while a single flat memcpy
ignores stride pitch and corrupts border row layout. Furthermore, downstream
operations like loop filtering and motion estimation expect edge pixels to be
extended from valid active frame boundaries; zero-filling buffer borders with
memset introduces artificial high-contrast edges and high gradients that distort
filter metrics and motion estimation calculations.

## Examples

Bad:

```cpp
// Anti-pattern: Zeroing borders or flat-copying across strides at negative offsets
if (yoffset < 0) {
  memset(dst_y, 0, -yoffset * sizeof(*dst_y));
  // or: memcpy(dst_y, src_ybc->y_buffer, -yoffset * sizeof(*dst_y));
}
memcpy(dst_y, src_ybc->y_buffer + yoffset, ystride * linestocopy);
```

Good:

```cpp
// Replicate top edge row stride-by-stride for negative border rows
const unsigned char *const top_row = src_ybc->y_buffer;
for (int i = yoffset; i < 0; i += ystride, --linestocopy) {
  memcpy(dst_y, top_row, ystride);
  dst_y += ystride;
  src_y += ystride;
}
memcpy(dst_y, src_y, ystride * linestocopy);
```
