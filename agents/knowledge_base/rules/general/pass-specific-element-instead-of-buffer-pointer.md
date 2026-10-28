---
name: libvpx/pass-specific-element-instead-of-buffer-pointer
description: When a helper function only accesses a single element from a buffer at caller coordinates, pass the specific element directly instead of the buffer pointer.
rubric: design
severity: should_fix
---

**Rule.** When defining helper functions that only read a single item or struct
from a buffer at specific coordinates, pass the element value or element pointer
directly rather than passing the whole buffer pointer along with index and
dimension parameters. Do not require callees to perform outer coordinate
indexing when only one element is needed.

**Why.** Passing only the required element simplifies the helper function
signature, eliminates unnecessary coupling to the outer buffer layout and
indexing arithmetic, and makes caller data dependencies explicit.

## Examples

Bad:

```cpp
// Callee takes entire buffer and coordinates, but only uses a single element
static MV get_smooth_motion_vector(MV *scaled_search_mf, const MV *tmp_mf,
                                   int rows, int cols, int row, int col) {
  const MV search_mv = scaled_search_mf[row * cols + col];
  // ...
}
```

Good:

```cpp
// Callee takes the specific element directly, simplifying signature and decoupling layout
static MV get_smooth_motion_vector(const MV search_mv, const MV *tmp_mf,
                                   int rows, int cols, int row, int col) {
  // search_mv is used directly without buffer coordinate indexing
  // ...
}
```
