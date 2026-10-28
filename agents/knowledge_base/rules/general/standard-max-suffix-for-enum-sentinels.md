---
name: libvpx/standard-max-suffix-for-enum-sentinels
description: When defining upper-bound, boundary, or sentinel enumerators in enum definitions, use standard suffixes such as _MAX rather than ad-hoc or colloquial names
rubric: readability
severity: should_fix
---

**Rule.** In enum declarations, use standardized naming conventions like
<PREFIX>_MAX (or <PREFIX>_COUNT / <PREFIX>_INVALID) for upper-bound, capacity,
or sentinel values. Avoid ad-hoc, informal, or colloquial suffixes such as
_NOT_CARE or _DONT_CARE.

**Why.** Standardized sentinel suffixes adhere to established C API idioms
across libvpx, making boundary conditions predictable and immediately
recognizable to callers and maintenance tools.

## Examples

Bad:

```cpp
typedef enum {
  LEVEL_UNKNOWN = 0,
  LEVEL_1 = 10,
  LEVEL_2 = 20,
  LEVEL_NOT_CARE = 255,
} VP9_LEVEL;
```

Good:

```cpp
typedef enum {
  LEVEL_UNKNOWN = 0,
  LEVEL_1 = 10,
  LEVEL_2 = 20,
  LEVEL_MAX = 255,
} VP9_LEVEL;
```
