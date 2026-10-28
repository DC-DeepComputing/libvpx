---
name: libvpx/use-coordinate-names-for-2d-loop-counters
description: When writing 2D pixel, buffer, or block loops, use coordinate names (x, y) or index pairs (i, j, r, c) rather than dimension abbreviations like w, h
rubric: readability
severity: nit
---

**Rule.** When writing loops that iterate over 2D buffers, image planes, or
block arrays, name iteration counters using coordinate variables (such as x and
y, r and c, or row and col) or generic index pairs (such as i, j). Reserve w, h,
width, and height exclusively for buffer or block dimension extents and boundary
limits.

**Why.** Reusing w and h as iteration counters creates confusion and variable
shadowing with dimension parameters (width, height, w, h). Using standard
coordinate variables (x, y or r, c) or index pairs (i, j) clearly distinguishes
mutable loop position coordinates from fixed dimension bounds.

## Examples

Bad:

```cpp
// Bad: using dimension abbreviations (w, h) for 2D loop counters
int w, h;
for (h = 0; h < height; ++h) {
  for (w = 0; w < width; w += 16) {
    process_block(src + h * stride + w);
  }
}
```

Good:

```cpp
// Good: use coordinate variables (x, y) or index pairs (r, c) for 2D loop counters
int x, y;
for (y = 0; y < height; ++y) {
  for (x = 0; x < width; x += 16) {
    process_block(src + y * stride + x);
  }
}
```
