---
name: libvpx/vp9/exclude-dc-coefficient-when-computing-transform-ac-variance
description: When computing AC variance, energy, or Wiener filtering over transform coefficients, exclude the DC coefficient at index 0 by starting iteration at index 1
rubric: correctness
severity: should_fix
---

**Rule.** When calculating AC variance, noise energy, or applying AC Wiener
filtering over transform-domain coefficients, exclude the DC coefficient at
index 0 by starting coefficient iterations from index 1. Do not include index 0
in AC variance sums or AC-specific filter calculations.

**Why.** In transform-domain representations (such as DCT or Walsh-Hadamard
transforms), coefficient index 0 represents the DC component (mean block signal
level), while indices 1 and above represent AC variations and noise. Including
the DC coefficient incorporates average block intensity into the AC variance
metric, distorting variance estimation and rate-distortion decisions.

## Examples

Bad:

```cpp
for (idx = 0; idx < coeff_count; ++idx) {
  int64_t sqr_coeff = (int64_t)coeff[idx] * coeff[idx];
  coeff[idx] = (int16_t)((sqr_coeff * coeff[idx]) /
                         (sqr_coeff + (int64_t)median_val * median_val));
  wiener_variance += (int64_t)coeff[idx] * coeff[idx];
}
```

Good:

```cpp
for (idx = 1; idx < coeff_count; ++idx) {
  int64_t sqr_coeff = (int64_t)coeff[idx] * coeff[idx];
  coeff[idx] = (int16_t)((sqr_coeff * coeff[idx]) /
                         (sqr_coeff + (int64_t)median_val * median_val));
  wiener_variance += (int64_t)coeff[idx] * coeff[idx];
}
```
