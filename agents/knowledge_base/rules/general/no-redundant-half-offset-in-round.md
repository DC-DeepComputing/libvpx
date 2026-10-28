---
name: libvpx/no-redundant-half-offset-in-round
description: When rounding floating-point numbers with standard library functions like round() or lround(), do not add +0.5 inside the function argument
rubric: correctness
severity: should_fix
---

**Rule.** Do not add + 0.5 (or + 0.5f) to the argument inside standard rounding
functions such as round(), lround(), llround(), or roundf(). Pass the unadjusted
floating-point expression directly to the rounding function.

**Why.** Standard rounding functions like round() already round half-way cases
away from zero. Retaining a manual integer rounding offset (+ 0.5) inside
round() causes double-rounding, erroneously shifting exact integer values and
thresholds upward (e.g., rounding 3.0 up to 4).

## Examples

Bad:

```cpp
// Adding 0.5 inside round() skews integer thresholds (e.g., 3.0 -> 4)
int interval = (int)round(min_gf_interval * factor / factor_safe + 0.5);
```

Good:

```cpp
int interval = (int)round(min_gf_interval * factor / factor_safe);
```
