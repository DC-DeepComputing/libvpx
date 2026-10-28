---
name: libvpx/compute-indexed-flags-inside-loop
description: When evaluating conditions or flags across an indexed range in a loop, compute per-element values inside the loop body instead of unrolling if checks
rubric: readability
severity: should_fix
---

**Rule.** When processing a range of indexed entities or array elements (such as
reference frames, planes, or modes) in an iteration loop, compute per-element
flags, preconditions, and skip conditions directly inside the loop body using
the loop index. Do not write repetitive, unrolled if checks or hardcoded array
index assignments immediately preceding the loop.

**Why.** Evaluating conditions inside the loop using the index variable
eliminates repetitive boilerplate, reduces code duplication, and ensures all
iterated elements are handled consistently when array bounds or usable ranges
change.

## Examples

Bad:

```cpp
if (!(cpi->ref_frame_flags & flag_list[LAST_FRAME]))
  skip_ref_find_pred[LAST_FRAME] = 1;
if (!(cpi->ref_frame_flags & flag_list[GOLDEN_FRAME]))
  skip_ref_find_pred[GOLDEN_FRAME] = 1;
if (!(cpi->ref_frame_flags & flag_list[ALTREF_FRAME]))
  skip_ref_find_pred[ALTREF_FRAME] = 1;

for (ref_frame = LAST_FRAME; ref_frame <= usable_ref_frame; ++ref_frame) {
  if (!skip_ref_find_pred[ref_frame]) {
    find_predictors(...);
  }
}
```

Good:

```cpp
for (ref_frame = LAST_FRAME; ref_frame <= usable_ref_frame; ++ref_frame) {
  skip_ref_find_pred[ref_frame] =
      !(cpi->ref_frame_flags & flag_list[ref_frame]);
  if (!skip_ref_find_pred[ref_frame]) {
    find_predictors(...);
  }
}
```
