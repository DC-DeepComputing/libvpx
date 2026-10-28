---
name: libvpx/omit-redundant-int-max-clamp-with-vpxmin
description: When clamping an int variable against an int64_t bound with VPXMIN, omit redundant INT_MAX clamps and use an explicit (int) cast
rubric: correctness
severity: should_fix
---

**Rule.** When bounding an integer variable against a wider int64_t value using
VPXMIN(target, max_val), do not add a redundant INT_MAX clamp to max_val when
target is already bounded within int range. Cast the VPXMIN expression
explicitly to (int) to handle the integer-promoted return type and satisfy
narrowing conversion checks.

**Why.** In C, VPXMIN promotes both operands to their common type (int64_t).
Because target is already a valid int, the minimum of target and max_val cannot
exceed target, making an extra INT_MAX clamp on max_val redundant. An explicit
(int) cast clearly documents that the result is in range and silences compiler
narrowing conversion warnings.

## Examples

Bad:

```cpp
// Redundantly clamping max_rate to INT_MAX before VPXMIN:
int64_t clamped_rate = VPXMIN(max_rate, (int64_t)INT_MAX);
target = VPXMIN(target, clamped_rate);

// Or omitting the explicit cast on the int64_t-promoted VPXMIN result:
target = VPXMIN(target, max_rate);  // Triggers compiler narrowing conversion warning
```

Good:

```cpp
// target is int, max_rate is int64_t.
// Because target is already within int range, VPXMIN cannot exceed target.
target = (int)VPXMIN(target, max_rate);
```
