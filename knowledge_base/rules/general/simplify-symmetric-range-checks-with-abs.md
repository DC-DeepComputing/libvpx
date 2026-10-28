---
name: libvpx/simplify-symmetric-range-checks-with-abs
description: When checking signed integers or coordinates against symmetric positive and negative bounds, use abs(val) <= limit instead of testing bounds separately
rubric: readability
severity: nit
---

**Rule.** When checking that a signed integer, motion vector, or delta value
lies within symmetric upper and lower limits (e.g., `-limit <= val && val <=
limit` or `<`), simplify the condition using `abs(val) <= limit` or `abs(val) <
limit`. Do not evaluate separate positive and negative boundary comparisons for
symmetric ranges.

**Why.** Evaluating positive and negative bounds in separate sub-expressions
duplicates variable references and limit constants, increasing condition
complexity and risk of sign or operator typos. Using `abs()` expresses the
symmetric magnitude constraint concisely and improves readability.

## Examples

Bad:

```cpp
if (candidate->mv[0].as_mv.row < INT16_MAX >> 1 &&
    candidate->mv[0].as_mv.row > -INT16_MAX >> 1 &&
    candidate->mv[0].as_mv.col < INT16_MAX >> 1 &&
    candidate->mv[0].as_mv.col > -INT16_MAX >> 1) {
  // Process candidate motion vector
}
```

Good:

```cpp
if (abs(candidate->mv[0].as_mv.row) <= INT16_MAX >> 1 &&
    abs(candidate->mv[0].as_mv.col) <= INT16_MAX >> 1) {
  // Process candidate motion vector
}
```
