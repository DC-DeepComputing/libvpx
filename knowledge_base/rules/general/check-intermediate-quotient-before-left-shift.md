---
name: libvpx/check-intermediate-quotient-before-left-shift
description: When scaling integers by dividing before left-shifting to avoid overflow, verify the quotient does not exceed INT_MAX >> shift and clamp before shifting
rubric: correctness
severity: should_fix
---

**Rule.** When scaling large integers by dividing before left-shifting to avoid
overflow (e.g., `(val / divisor) << shift`), verify whether `val / divisor`
still exceeds `INT_MAX >> shift` and clamp the result to `INT_MAX` before
performing the left shift. Do not assume dividing by a variable divisor
guarantees the quotient fits within the shift threshold when the divisor can be
small (e.g., single macroblock or unit count).

**Why.** Dividing a large value by a divisor before left-shifting reduces the
magnitude, but if the divisor is small (such as `MBs == 1`), the intermediate
quotient may still exceed `INT_MAX >> shift`. Shifting such a value left results
in signed integer overflow, which causes undefined behavior in C/C++ and can
produce corrupted or negative values.

## Examples

Bad:

```cpp
if (target_bits_per_frame > (INT_MAX >> BPER_MB_NORMBITS)) {
  // BAD: If divisor is small (e.g. MBs == 1), quotient still exceeds limit and left-shift overflows
  target_bits_per_mb = (target_bits_per_frame / cpi->common.MBs)
                       << BPER_MB_NORMBITS;
}
```

Good:

```cpp
if (target_bits_per_frame > (INT_MAX >> BPER_MB_NORMBITS)) {
  // GOOD: Check if quotient still exceeds threshold before shifting, and clamp to INT_MAX
  int temp = target_bits_per_frame / cpi->common.MBs;
  if (temp > (INT_MAX >> BPER_MB_NORMBITS)) {
    target_bits_per_mb = INT_MAX;
  } else {
    target_bits_per_mb = temp << BPER_MB_NORMBITS;
  }
}
```
