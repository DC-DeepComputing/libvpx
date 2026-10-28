---
name: libvpx/assert-strictly-positive-denominators
description: When asserting preconditions on denominator or divisor parameters in division and rational helpers, check strictly positive (> 0) instead of non-negative (>= 0)
rubric: correctness
severity: should_fix
---

**Rule.** When writing precondition assertions for denominator or divisor
parameters in rational arithmetic, greatest common divisor (GCD), or division
helper functions, assert that the value is strictly positive (`> 0`) rather than
non-negative (`>= 0`). Never permit zero as a valid denominator in
preconditions.

**Why.** Dividing by zero or taking the remainder with a zero divisor causes
undefined behavior and runtime crashes. Checking `b >= 0` erroneously accepts
zero, allowing invalid rational states and division-by-zero bugs to slip past
assertions.

## Examples

Bad:

```cpp
static INLINE int gcd(int64_t a, int b) {
  int r;
  assert(a >= 0);
  assert(b >= 0);  // Bug: b == 0 causes division by zero or invalid state.
  while (b != 0) {
    r = (int)(a % b);
    a = b;
    b = r;
  }
  return (int)a;
}
```

Good:

```cpp
static INLINE int gcd(int64_t a, int b) {
  int r;
  assert(a >= 0);
  assert(b > 0);  // Enforce strictly positive denominator/divisor.
  while (b != 0) {
    r = (int)(a % b);
    a = b;
    b = r;
  }
  return (int)a;
}
```
