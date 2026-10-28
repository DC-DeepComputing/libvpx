---
name: libvpx/encapsulate-signed-casts-in-math-helpers
description: When writing math helper functions that perform unsigned arithmetic on signed values, accept signed parameters directly, cast internally, and assert range preconditions
rubric: readability
severity: should_fix
---

**Rule.** When helper functions perform unsigned arithmetic on signed quantities
(such as converting negative signed differences to unsigned integers for modular
squaring or sanitizer-clean overflow), design the helper to accept the signed
type directly and perform the conversion internally. Document the function's
input assumptions in a comment and enforce range preconditions with assert()
(e.g., assert(abs(value) <= UINT16_MAX)).

**Why.** Accepting the caller's actual signed type avoids polluting call sites
with explicit casts and clarifies the function interface. Performing the cast
internally allows the helper to validate and assert range preconditions
directly, preventing incorrect results and catching out-of-range inputs during
testing.

## Examples

Bad:

```cpp
static VPX_NO_UNSIGNED_OVERFLOW_CHECK uint32_t square_value(uint32_t value) {
  return value * value;
}

// Call site:
*sse += square_value((uint32_t)diff);
```

Good:

```cpp
// This function assumes `abs(value) <= UINT16_MAX`, as the only overflow
// that is expected is from casting negative signed integers to unsigned
// and squaring the result.
static VPX_NO_UNSIGNED_OVERFLOW_CHECK uint32_t square_value(int value) {
  assert(abs(value) <= UINT16_MAX);
  const uint32_t unsigned_value = (uint32_t)value;
  return unsigned_value * unsigned_value;
}

// Call site:
*sse += square_value(diff);
```
