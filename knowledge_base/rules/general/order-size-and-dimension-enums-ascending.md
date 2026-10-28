---
name: libvpx/order-size-and-dimension-enums-ascending
description: When defining enums representing discrete block sizes, dimensions, or quantitative tiers, order enumerators in natural ascending order from smallest to largest.
rubric: readability
severity: nit
---

**Rule.** When defining enum types or constant enumerations representing
discrete block sizes, dimensions, or quantitative tiers, order the enumerators
in natural ascending order (from smallest to largest). Do not declare larger
dimensions or higher tiers before smaller ones unless an external specification
dictates fixed index values.

**Why.** Ordering enumerators from smallest to largest establishes an intuitive
progression consistent with libvpx conventions (such as BLOCK_4X4 through
BLOCK_64X64) and avoids confusion when iterating or indexing tables across
dimensions.

## Examples

Bad:

```cpp
typedef enum {
  SKIN_16X16,
  SKIN_8X8
} SKIN_DETECTION_BLOCK_SIZE;
```

Good:

```cpp
typedef enum {
  SKIN_8X8,
  SKIN_16X16
} SKIN_DETECTION_BLOCK_SIZE;
```
