---
name: libvpx/extract-repeated-dimension-conversion-helpers
description: When performing repeated coordinate, unit, or dimension conversions, extract inline shift and rounding arithmetic into documented helper functions
rubric: design
severity: should_fix
---

**Rule.** Encapsulate repeated coordinate, unit, and dimension conversion
calculations (such as converting pixel dimensions to block counts via ceiling
addition and bit shifts) into named, static inline helper functions. Document
the conversion rationale, rounding offset, and scale factor at the helper
definition instead of repeating open-coded arithmetic expressions across call
sites.

**Why.** Duplicating inline bitwise arithmetic and rounding offsets (e.g., `(dim
+ 3) >> 2`) obscures semantic intent, increases maintenance overhead, and risks
inconsistencies across call sites. Encapsulating conversions in documented
helper functions clarifies unit boundaries and makes the arithmetic
self-describing.

## Examples

Bad:

```cpp
// Duplicated inline bitwise conversions across multiple call sites:
const int num_unit_cols = (cm->width + 3) >> 2;
const int unit_height = (cm->height + 3) >> 2;
int sb_offset = ((cm->mi_cols + 7) >> 3) * (mi_row >> 3);
```

Good:

```cpp
// Returns the number of 4x4 block units for a given pixel dimension (rounding up).
static INLINE int get_num_4x4_units(int dim) {
  return (dim + 3) >> 2;
}

const int num_unit_cols = get_num_4x4_units(cm->width);
const int unit_height = get_num_4x4_units(cm->height);
```
