---
name: libvpx/vp9/avoid-redundant-context-struct-fields
description: In encoder contexts like VP9_COMP, access canonical sub-structures like cpi->common or derive properties like image_bps instead of adding redundant fields
rubric: design
severity: should_fix
---

**Rule.** Do not add redundant fields or cached state variables to encoder
context structs (such as VP9_COMP or VP8_COMP) when underlying properties (such
as bit depth, chroma subsampling, or frame dimensions) already exist in embedded
sub-structures like cpi->common (VP9_COMMON). Compute or access properties
directly from the canonical sub-structures (e.g., derive image bits-per-sample
via (8 + 2 * (8 >> (cm->subsampling_x + cm->subsampling_y))) * (1 +
(cm->bit_depth > 8))). Additionally, do not redundantly clamp derived
bits-per-sample with VPXMAX(8, ...), as all standard YUV formats require at
least 12 bits per sample.

**Why.** Adding redundant struct fields duplicates state that already has a
canonical source of truth in VP9_COMMON, risking desynchronization bugs,
increasing maintenance overhead, and needlessly inflating context struct size.
Calculating properties directly ensures consistency across format changes and
eliminates unnecessary runtime clamping since YUV 4:2:0 through 4:4:4 guarantees
at least 12 bits per sample.

## Examples

Bad:

```cpp
// Storing and plumbing redundant image_bps state in VP9_COMP:
typedef struct VP9_COMP {
  ...
  int image_bps;
} VP9_COMP;

// Redundantly clamping against 8 during buffer allocation:
int factor = VPXMAX(8, cpi->image_bps);
return 4 + (int64_t)cpi->oxcf.width * cpi->oxcf.height * factor / 8;
```

Good:

```cpp
// Derive image bits-per-sample directly from cpi->common without clamping:
const VP9_COMMON *const cm = &cpi->common;
const int image_bps =
    (8 + 2 * (8 >> (cm->subsampling_x + cm->subsampling_y))) *
    (1 + (cm->bit_depth > 8));
return 4 + (int64_t)cpi->oxcf.width * cpi->oxcf.height * image_bps / 8;
```
