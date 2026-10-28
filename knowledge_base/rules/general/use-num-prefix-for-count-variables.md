---
name: libvpx/use-num-prefix-for-count-variables
description: When naming variables, struct members, or parameters representing counts or quantities, use the standard num_ prefix instead of no_of_ or number_of_
rubric: readability
severity: nit
---

**Rule.** Use the standard num_ prefix (e.g., num_tiles_done, num_workers,
num_rows) rather than non-standard phrases like no_of_ or number_of_ when naming
variables, struct members, and function parameters that represent counts or
quantities.

**Why.** In libvpx and standard C/C++ conventions, num_ is the established,
uniform identifier prefix for counts and quantities. Non-standard phrases such
as no_of_ introduce inconsistency across headers and source files and impair
codebase searchability.

## Examples

Bad:

```cpp
struct TileSyncData {
  int *no_of_tiles_done;
  int number_of_workers;
};
```

Good:

```cpp
struct TileSyncData {
  int *num_tiles_done;
  int num_workers;
};
```
