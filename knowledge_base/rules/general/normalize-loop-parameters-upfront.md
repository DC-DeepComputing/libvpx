---
name: libvpx/normalize-loop-parameters-upfront
description: When processing loops differ only by uniform versus distinct parameters, normalize parameters before the loop rather than branching inside it or creating duplicate helpers
rubric: design
severity: should_fix
---

**Rule.** When multiple code paths in a processing loop differ only by whether
input parameters are distinct or identical (e.g., separate vs. uniform weights,
strides, or coefficients), normalize the parameters before the loop rather than
branching repeatedly inside the loop or maintaining duplicate helper functions
for uniform and non-uniform cases.

**Why.** Branching inside an inner processing loop to handle uniform vs.
separate parameters introduces unnecessary branch overhead and leads to
duplicate helper variants. Normalizing parameters upfront unifies the loop logic
into a single path and eliminates redundant helper functions.

## Examples

Bad:

```cpp
// Redundant branching and separate helpers for uniform vs separate weights
if (blk_fw) {
  sum_row_first =
      average_8(sum_row_first, &mul_first, strength, rounding, &weight_first);
  sum_row_second = average_8(sum_row_second, &mul_second, strength, rounding,
                             &weight_second);
} else {
  average_16(&sum_row_first, &sum_row_second, &mul_first, &mul_second,
             strength, rounding, &weight_first);
}
```

Good:

```cpp
// Normalize parameters upfront so the inner loop uses a single helper path
if (!blk_fw) {
  weight_first = _mm_set1_epi16(top_weight);
  weight_second = weight_first;
}
...
sum_row_first =
    average_8(sum_row_first, &mul_first, strength, rounding, &weight_first);
sum_row_second = average_8(sum_row_second, &mul_second, strength, rounding,
                           &weight_second);
```
