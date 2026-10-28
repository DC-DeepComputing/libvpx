---
name: libvpx/break-early-from-short-circuitable-loops
description: When evaluating aggregate boolean predicates, search conditions, or buffer validation checks in a loop, return or break early once determined
rubric: safety
severity: should_fix
---

**Rule.** Break early or return immediately from loops that evaluate aggregate
boolean predicates (e.g., checking whether all elements, any element, or no
elements satisfy a condition), perform searches, or validate buffers for
invalid/out-of-range values as soon as the final result or first failure is
determined. Avoid continuing loop iterations or accumulating results across
elements (such as with bitwise OR operations `ret |= ...` or unshortcircuited
flag updates) when subsequent passes cannot alter the outcome and perform no
required side effects.

**Why.** Continuing to iterate over remaining elements after an aggregate
boolean condition, search, or validation check is already decided wastes CPU
cycles and memory bandwidth. Short-circuiting with an early `break` or `return`
eliminates redundant computation in performance-critical codec execution loops
while producing the exact same result.

## Examples

Bad:

```cpp
// Unnecessary iterations after flag is determined
int all_layers_drop = 1;
for (i = 0; i < cpi->svc.spatial_layer_id; ++i) {
  if (cpi->svc.drop_spatial_layer[i] == 0) {
    all_layers_drop = 0;
  }
}

// Redundant scans across entire buffer using bitwise OR
static INLINE int detect_invalid_input(const tran_low_t *input, int size) {
  int i, ret = 0;
  for (i = 0; i < size; ++i) ret |= abs(input[i]) >= VALID_MAGNITUDE_RANGE;
  return ret;
}
```

Good:

```cpp
// Breaks immediately once the outcome is decided
int all_layers_drop = 1;
for (i = 0; i < cpi->svc.spatial_layer_id; ++i) {
  if (cpi->svc.drop_spatial_layer[i] == 0) {
    all_layers_drop = 0;
    break;
  }
}

// Returns immediately upon finding the first invalid element
static INLINE int detect_invalid_input(const tran_low_t *input, int size) {
  int i;
  for (i = 0; i < size; ++i) {
    if (abs(input[i]) >= VALID_MAGNITUDE_RANGE) return 1;
  }
  return 0;
}
```
