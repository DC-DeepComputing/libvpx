---
name: libvpx/document-or-convert-fixed-point-units-in-api-structs
description: When exposing internal fixed-point, sub-pixel, or scaled quantities in struct definitions or APIs, convert them to physical units or document their precision
rubric: readability
severity: should_fix
---

**Rule.** When exposing internal fixed-point, sub-pixel, or scaled quantities
(such as motion vector offsets, timestamps, or bitrates) in API structures,
configuration structs, or statistics outputs, convert them to standard physical
units (e.g., double in pixel or second units) or explicitly document their exact
unit, scale factor, and sub-pixel precision in field comments.

**Why.** Internal codec representations frequently store quantities in scaled
fixed-point integers (such as 1/8th sub-pixel units for motion vectors).
Exposing raw integer fields without explicit unit or scale documentation leads
to caller confusion, incorrect unit conversions, and API integration bugs.

## Examples

Bad:

```cpp
struct MotionVectorInfo {
  int mv_row[2];     // The row offset of motion vectors.
  int mv_column[2];  // The column offset of motion vectors.
};
```

Good:

```cpp
struct MotionVectorInfo {
  // The row offset of motion vectors in the unit of pixel.
  double mv_row[2];
  // The column offset of motion vectors in the unit of pixel.
  double mv_column[2];
};
```
