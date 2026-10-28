---
name: libvpx/preserve-qsort-comparator-polarity
description: When writing or refactoring qsort comparator callbacks using three-way comparison expressions, verify the return sign matches the intended ascending or descending sort order.
rubric: correctness
severity: should_fix
---

**Rule.** When implementing or refactoring qsort comparator callbacks or
three-way comparison helpers, verify that the return sign matches the intended
sort direction: return positive when the first argument exceeds the second for
ascending order ((a > b) - (a < b)), and return positive when the first argument
is smaller for descending order ((a < b) - (a > b) or (b > a) - (b < a)).

**Why.** Standard qsort sorts elements based on whether the comparator returns a
negative, zero, or positive integer. Using an ascending comparison expression
((a > b) - (a < b)) when descending order is required silently reverses the sort
order, causing algorithmic and frame decoding logic errors.

## Examples

Bad:

```cpp
// Sorts in descending order (hazard: inverted polarity sorts in ascending order)
static int compare_tile_buffers(const void *a, const void *b) {
  const TileBuffer *const buf_a = (const TileBuffer *)a;
  const TileBuffer *const buf_b = (const TileBuffer *)b;
  return (buf_a->size > buf_b->size) - (buf_a->size < buf_b->size);
}
```

Good:

```cpp
// Sorts in descending order
static int compare_tile_buffers(const void *a, const void *b) {
  const TileBuffer *const buf_a = (const TileBuffer *)a;
  const TileBuffer *const buf_b = (const TileBuffer *)b;
  return (buf_a->size < buf_b->size) - (buf_a->size > buf_b->size);
}
```
