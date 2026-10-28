---
name: libvpx/vp9/vp9-encoder-context-parameter-constness
description: When declaring or passing VP9 encoder context parameters, use 'VP9_COMP *const cpi' rather than 'const VP9_COMP *const cpi' to match encoder conventions
rubric: readability
severity: should_fix
---

**Rule.** In VP9 encoder routines, declare encoder context parameters as
'VP9_COMP *const cpi' (or 'VP9_COMP *cpi') rather than 'const VP9_COMP *const
cpi' or 'const VP9_COMP *cpi', even for read-only helper functions. Keep the
pointed-to encoder structure non-const to stay consistent with prevailing
encoder function signatures.

**Why.** In the VP9 encoder codebase, encoder context instances are
conventionally passed as mutable pointers ('VP9_COMP *const cpi'). Adding const
qualification to the pointed-to VP9_COMP struct breaks consistency across
encoder routines and creates signature mismatches when passing cpi to nested
encoder helpers.

## Examples

Bad:

```cpp
static int is_superblock_flat_static(const VP9_COMP *const cpi, int sb_row_index,
                                     int sb_col_index) {
  const uint8_t *src_y = cpi->Source->y_buffer;
  // ...
}
```

Good:

```cpp
static int is_superblock_flat_static(VP9_COMP *const cpi, int sb_row_index,
                                     int sb_col_index) {
  const uint8_t *src_y = cpi->Source->y_buffer;
  // ...
}
```
