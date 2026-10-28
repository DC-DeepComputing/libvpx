---
name: libvpx/validate-float-isfinite-before-int-cast
description: When validating floating-point inputs before range checking or integer conversion, check isfinite() to reject NaN and infinite values
rubric: correctness
severity: should_fix
---

**Rule.** When validating floating-point values from external inputs or
statistics before bounds checking or casting to integer types, explicitly verify
that the value is finite using isfinite() (or std::isfinite). Do not rely solely
on relational operators (such as <, >, <=, >=) for range validation, as
comparisons against NaN always evaluate to false and allow NaN values to bypass
bounds checks into integer conversions.

**Why.** In C and C++, converting NaN or infinity to an integer triggers
undefined behavior. Because all relational comparisons against NaN evaluate to
false, standard range checks like val < min || val > max fail to detect NaN,
permitting invalid floating-point values to reach unchecked integer casts.

## Examples

Bad:

```cpp
const double count_rounded = stats->count + 0.5;
// Relational comparisons against NaN evaluate to false, bypassing bounds check.
if (count_rounded < 0 || count_rounded > (double)UINT_MAX ||
    (unsigned int)count_rounded != n_packets - 1) {
  ERROR("rc_twopass_stats_in missing EOS stats packet");
}
```

Good:

```cpp
const double count_rounded = stats->count + 0.5;
// Explicitly reject NaN and infinity before integer casting.
if (!isfinite(stats->count) || stats->count < 0.0 ||
    count_rounded > (double)UINT_MAX ||
    (unsigned int)count_rounded != n_packets - 1) {
  ERROR("rc_twopass_stats_in missing EOS stats packet");
}
```
