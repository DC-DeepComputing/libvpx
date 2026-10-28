---
name: libvpx/inclusive-bounds-for-integer-scaling-limits
description: When guarding integer scaling or shifts against range limits (MAX >> shift or MAX / factor), use inclusive bounds (<=) for safe values and strict inequality (>) for overflow triggers
rubric: correctness
severity: should_fix
---

**Rule.** When guarding integer scaling, multiplication, or left-shift
operations (`x << shift`, `val * factor`) against maximum range limits (such as
`INT16_MAX >> 1`, `INT_MAX >> shift`, or `MAX / factor`):
1. When checking that a value falls within the safe range to scale, use inclusive comparisons (`<=` and `>=`, or `abs(val) <= MAX >> shift` / `MAX / factor`) rather than strict inequalities (`<` and `>`).
2. When checking whether a value will overflow before left-shifting to trigger a fallback path, use strict inequality (`x > (MAX >> shift)`) rather than `>= (MAX >> shift)`.
For signed types, also verify non-negativity or negative limits to avoid undefined shift/overflow behavior.

**Why.** The threshold `MAX >> shift` (or `MAX / factor`) represents the exact
boundary value that can be safely scaled without overflow (since `(MAX >> shift)
<< shift <= MAX` and `(MAX / factor) * factor <= MAX`). Using strict inequality
(`<`) for safe-range guards prematurely rejects valid maximum boundary inputs
(such as motion vector coordinates). Conversely, using inclusive inequality
(`>=`) for overflow guards falsely triggers fallback paths on valid boundary
values, causing unnecessary precision loss.

## Examples

Bad:

```cpp
// BAD: Strict inequalities reject valid boundary values (e.g., INT16_MAX >> 1).
if (candidate->mv[0].as_mv.row < INT16_MAX >> 1 &&
    candidate->mv[0].as_mv.row > -INT16_MAX >> 1) {
  base_mv->as_mv.row = candidate->mv[0].as_mv.row * 2;
}

// BAD: Inclusive '>=' triggers fallback path on exact boundary value.
if (target_bits_per_frame >= (INT_MAX >> BPER_MB_NORMBITS)) {
  target_bits_per_mb = (target_bits_per_frame / cpi->common.MBs) << BPER_MB_NORMBITS;
} else {
  target_bits_per_mb = (target_bits_per_frame << BPER_MB_NORMBITS) / cpi->common.MBs;
}
```

Good:

```cpp
// GOOD: Inclusive '<=' allows all valid boundary values fitting in INT16.
if (abs(candidate->mv[0].as_mv.row) <= INT16_MAX >> 1) {
  base_mv->as_mv.row = candidate->mv[0].as_mv.row * 2;
}

// GOOD: Strict '>' triggers fallback only when value strictly exceeds boundary.
if (target_bits_per_frame > (INT_MAX >> BPER_MB_NORMBITS)) {
  target_bits_per_mb = (target_bits_per_frame / cpi->common.MBs) << BPER_MB_NORMBITS;
} else {
  target_bits_per_mb = (target_bits_per_frame << BPER_MB_NORMBITS) / cpi->common.MBs;
}
```
