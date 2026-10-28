---
name: libvpx/omit-type-prefix-in-scoped-enums
description: When defining enumerators in scoped enums (enum class), omit the enum type name or abbreviation prefix to avoid redundant repetition at call sites
rubric: readability
severity: should_fix
---

**Rule.** Do not prefix enumerators of a scoped enum (`enum class` or `enum
struct`) with the enum type's name or abbreviation. Because scoped enums require
explicit type qualification at points of use, omit type prefixes from enumerator
names to avoid redundant stutter.

**Why.** Scoped enums establish their own scope and must be qualified with the
enum type name at use sites. Repeating the enum type name or its abbreviation in
each enumerator name adds visual noise and redundant repetition without
improving clarity.

## Examples

Bad:

```cpp
enum class RcFrameType {
  kRcKeyFrame = 0,
  kRcInterFrame = 1,
};
// Use site: RcFrameType::kRcKeyFrame
```

Good:

```cpp
enum class RcFrameType {
  kKeyFrame = 0,
  kInterFrame = 1,
};
// Use site: RcFrameType::kKeyFrame
```
