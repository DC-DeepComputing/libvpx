---
name: libvpx/explicit-count-for-variable-array-elements
description: When designing structs with fixed-size arrays holding a variable number of elements, include an explicit count field rather than sentinel values
rubric: readability
severity: should_fix
---

**Rule.** When designing data structures or structs containing fixed-capacity
arrays that hold a variable number of active elements, include an explicit count
field (such as 'mv_count' or 'num_entries') to track the valid element count. Do
not require consumers to deduce the count from sentinel values (like -1 or
nullptr) in array slots.

**Why.** Explicit count fields make data structures self-describing, eliminate
ambiguity across different frame or block modes, and simplify iteration and
validation without relying on sentinel checking or assumptions about unused
array entries.

## Examples

Bad:

```cpp
struct MotionVectorInfo {
  // If the second motion vector does not exist, ref_frame is -1.
  int ref_frame[2];
  int mv_row[2];
  int mv_column[2];
};
```

Good:

```cpp
struct MotionVectorInfo {
  // Number of valid motion vectors: 0 for key frames, 1 or 2 for inter frames.
  int mv_count;
  RefFrameType ref_frame[2];
  double mv_row[2];
  double mv_column[2];
};
```
