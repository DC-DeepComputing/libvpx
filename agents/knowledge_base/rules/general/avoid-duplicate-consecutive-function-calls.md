---
name: libvpx/avoid-duplicate-consecutive-function-calls
description: When initializing buffers or executing subroutines, avoid duplicate consecutive calls to the same initialization or helper function on identical arguments
rubric: readability
severity: nit
---

**Rule.** Do not invoke identical initialization functions, buffer setup
helpers, or subroutines multiple consecutive times on the same struct instance
or arguments. Eliminate duplicate sequential calls that result from copy-paste
errors or redundant refactoring.

**Why.** Duplicate consecutive calls to initialization or setup routines are
redundant, add unnecessary CPU overhead, risk resetting or reallocating
already-initialized buffers, and usually indicate accidental copy-paste errors.

## Examples

Bad:

```cpp
// Redundant duplicate consecutive call to initialization routine.
init_sb_mul_scale_buffer(cpi);
init_sb_mul_scale_buffer(cpi);
```

Good:

```cpp
// Initialize the buffer once.
init_sb_mul_scale_buffer(cpi);
```
