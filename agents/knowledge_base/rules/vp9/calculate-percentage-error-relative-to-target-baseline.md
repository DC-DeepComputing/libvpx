---
name: libvpx/vp9/calculate-percentage-error-relative-to-target-baseline
description: When calculating percentage difference or relative error against a baseline target in rate control, divide by the target value rather than the average
rubric: correctness
severity: should_fix
---

**Rule.** When calculating the percentage difference or relative error of an
actual measurement against a target baseline (such as actual_bits versus
target_bits in rate control), divide by the target value rather than the average
of the two values. Ensure the target divisor is validated or guarded against
zero before dividing.

**Why.** Rate control decisions and tolerance thresholds measure deviation
relative to the planned target baseline. Dividing by the average of target and
actual values distorts the relative error when actual values deviate
significantly from target, skewing rate control recode decisions.

## Examples

Bad:

```cpp
static double get_bits_percent_diff(int target_bits, int actual_bits) {
  double diff =
      abs(target_bits - actual_bits) * 2. / (target_bits + actual_bits);
  diff *= 100;
  return diff;
}
```

Good:

```cpp
static double get_bits_percent_diff(int target_bits, int actual_bits) {
  double diff = 0.0;
  if (target_bits > 0) {
    diff = (double)abs(target_bits - actual_bits) * 100.0 / target_bits;
  }
  return diff;
}
```
