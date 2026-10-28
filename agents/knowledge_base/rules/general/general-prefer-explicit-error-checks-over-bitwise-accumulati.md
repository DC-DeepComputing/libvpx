---
name: libvpx/general-prefer-explicit-error-checks-over-bitwise-accumulati
description: When checking fallible function calls in loops or sequential blocks, use explicit if (!func()) error assignments instead of bitwise-OR expressions like error |= !func().
rubric: readability
severity: nit
---

**Rule.** Structure error checks and error-flag assignments explicitly using
conditional statements (e.g., if (!func()) { error = 1; continue; }) rather than
bitwise-OR error accumulation (e.g., error |= !func();). Maintain consistent
error-signaling and early-exit control flow across all branches within the same
loop or function.

**Why.** Bitwise-OR error accumulation (error |= !func();) obscures failure
points, delays error handling, and creates inconsistent control flow when mixed
with conditional branching. Explicit if (!func()) checks make error propagation
immediately visible and maintain clear, predictable branch behavior.

## Examples

Bad:

```cpp
for (int i = 0; i < num_workers; ++i) {
  error |= !winterface->sync(worker);
  if (error) continue;
  process_worker_output(worker);
}
```

Good:

```cpp
for (int i = 0; i < num_workers; ++i) {
  if (!winterface->sync(worker)) {
    error = 1;
    continue;
  }
  process_worker_output(worker);
}
```
