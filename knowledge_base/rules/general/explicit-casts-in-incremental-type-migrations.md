---
name: libvpx/explicit-casts-in-incremental-type-migrations
description: When changing function return types or numeric variables incrementally, explicitly cast return values at call sites still using the old type
rubric: readability
severity: should_fix
---

**Rule.** When changing a function's return type or variable representation
(such as converting between floating-point double/float and integer
int64_t/int), explicitly cast return values at call sites where surrounding code
has not yet migrated to the new type, unless all callers and receiving variables
are updated in a single pass.

**Why.** Implicit conversions between floating-point and integer types or
between differing integer widths can cause silent precision loss, unexpected
truncation, or compiler conversion warnings (-Wconversion). Explicit casts
clarify intentional impedance matching during multi-step refactoring patches.

## Examples

Bad:

```cpp
// Function return type migrated to int64_t, but bestsme is still double
bestsme = full_pixel_exhaustive_new(cpi, x, best_mv, fn_ptr, best_mv,
                                    lambda, nb_full_mvs, full_mv_num);
```

Good:

```cpp
// Explicit cast clarifies interim type conversion during incremental migration
bestsme = (double)full_pixel_exhaustive_new(cpi, x, best_mv, fn_ptr,
                                            best_mv, lambda, nb_full_mvs,
                                            full_mv_num);
```
