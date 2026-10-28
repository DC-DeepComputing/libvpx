---
name: libvpx/assert-sentinel-updated-before-use
description: When initializing selection or candidate variables with invalid sentinel values (like *_COUNT or -1), assert they are overwritten before consuming or storing the result
rubric: correctness
severity: should_fix
---

**Rule.** When initializing a selection or candidate variable to an invalid
sentinel value (such as *_COUNT, *_MODE_COUNT, INVALID_*, kInvalid*, -1, or
INT_MAX) to mark an unset state before search loops or selection logic, assert
before reading, passing, or storing the value that it has been overwritten with
a valid value on all control flow paths, or explicitly handle fallback paths.

**Why.** Initializing variables with sentinel values suppresses compiler
uninitialized-variable warnings without guaranteeing that search iterations
executed or branching logic selected a valid option. If all search candidates
are skipped or rejected, consuming the unmodified sentinel value causes
out-of-bounds indexing or silent state corruption. Asserting before use catches
unhandled execution paths and search logic regressions early in debug builds.

## Examples

Bad:

```cpp
MB_PREDICTION_MODE best_mode = MB_MODE_COUNT;
for (int mode = DC_PRED; mode <= TM_PRED; ++mode) {
  if (this_rd < best_rd) {
    best_rd = this_rd;
    best_mode = (MB_PREDICTION_MODE)mode;
  }
}
xd->mode_info_context->mbmi.uv_mode = best_mode;
```

Good:

```cpp
MB_PREDICTION_MODE best_mode = MB_MODE_COUNT;
for (int mode = DC_PRED; mode <= TM_PRED; ++mode) {
  if (this_rd < best_rd) {
    best_rd = this_rd;
    best_mode = (MB_PREDICTION_MODE)mode;
  }
}
assert(best_mode != MB_MODE_COUNT);
xd->mode_info_context->mbmi.uv_mode = best_mode;
```
