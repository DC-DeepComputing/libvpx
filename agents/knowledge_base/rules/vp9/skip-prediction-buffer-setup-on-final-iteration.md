---
name: libvpx/vp9/skip-prediction-buffer-setup-on-final-iteration
description: When allocating or preparing scratch prediction buffers after updating the best candidate in search loops, skip buffer setup on the final iteration
rubric: safety
severity: should_fix
---

**Rule.** In encoder candidate search loops (such as mode or interpolation
filter searches) where finding a new best candidate acquires a scratch
prediction buffer for evaluating subsequent candidates, guard buffer
allocation/acquisition with a check ensuring the current iteration is not the
last (e.g., filter != filter_end). Do not unconditionally allocate or set up
scratch destination buffers on the final loop iteration when no further
candidate evaluations will take place.

**Why.** Acquiring and configuring a new scratch prediction buffer on the final
loop iteration wastes CPU cycles because no further iterations will evaluate
candidates or use the newly prepared buffer, adding unnecessary overhead to
innermost rate-distortion search loops.

## Examples

Bad:

```cpp
if (cost < best_cost) {
  best_filter = filter;
  if (reuse_inter_pred) {
    if (*this_mode_pred != current_pred) {
      free_pred_buffer(*this_mode_pred);
      *this_mode_pred = current_pred;
    }
    current_pred = &tmp[get_pred_buffer(tmp, 3)];
    pd->dst.buf = current_pred->data;
    pd->dst.stride = bw;
  }
}
```

Good:

```cpp
if (cost < best_cost) {
  best_filter = filter;
  if (reuse_inter_pred) {
    if (*this_mode_pred != current_pred) {
      free_pred_buffer(*this_mode_pred);
      *this_mode_pred = current_pred;
    }
    if (filter != filter_end) {
      current_pred = &tmp[get_pred_buffer(tmp, 3)];
      pd->dst.buf = current_pred->data;
      pd->dst.stride = bw;
    }
  }
}
```
