---
name: libvpx/use-named-constants-and-compile-time-assertions
description: Use named constants instead of magic numbers for scaling factors and timebases, enforcing constant arithmetic invariants with compile-time assertions at usage sites
rubric: readability
severity: should_fix
---

**Rule.** Use named constants (such as #define macros or enum values) rather
than hardcoded numeric literals for scaling factors, timebases, and buffer
dimensions. When calculations, unit conversions, or arithmetic operations rely
on specific invariant properties, ranges, or relationships of compile-time
constants (such as divisibility, scaling factors, or bounds), verify those
assumptions with compile-time assertions (such as VPX_STATIC_ASSERT) directly at
dependent usage sites instead of relying solely on comments, unverified
assumptions, or placing checks only at constant definitions.

**Why.** Hardcoding magic numbers obscures code intent and risks inconsistency
when constants or specifications change across modules. Placing compile-time
assertions directly at dependent calculation sites ensures that future
modifications to constants immediately trigger compile errors at the affected
operations rather than silently introducing arithmetic errors or runtime bugs.

## Examples

Bad:

```cpp
// Bad: Hardcoded magic numbers
priv->timestamp_ratio.num *= 10000000;

// Bad: Relying solely on comments for arithmetic assumptions
/* Assumes TICKS_PER_SEC > 1000000 and TICKS_PER_SEC % 1000000 == 0 */
duration_us = duration * (uint64_t)ctx->timestamp_ratio.num /
              ((uint64_t)ctx->timestamp_ratio.den * 10u);
```

Good:

```cpp
// Good: Named constant and compile-time assertion at usage site
priv->timestamp_ratio.num *= TICKS_PER_SEC;

VPX_STATIC_ASSERT(TICKS_PER_SEC > 1000000 &&
                  (TICKS_PER_SEC % 1000000) == 0);
duration_us = duration * (uint64_t)ctx->timestamp_ratio.num /
              ((uint64_t)ctx->timestamp_ratio.den * (TICKS_PER_SEC / 1000000));
```
