---
name: libvpx/declare-void-return-for-unused-helper-values
description: When defining helper functions whose outputs are passed via pointer out-parameters and callers ignore return values, declare the return type as void
rubric: readability
severity: should_fix
---

**Rule.** Do not return a value from helper functions when callers ignore the
return value and results are produced via pointer out-parameters or side
effects. Declare the function return type as void and omit trailing return
statements.

**Why.** Returning an unused value constitutes dead code and misleads readers or
callers into expecting semantic significance from the return value when data is
already communicated via output parameters.

## Examples

Bad:

```cpp
static int pick_kf_q_bound_two_pass(const VP9_COMP *cpi, int *bottom_index,
                                    int *top_index) {
  ...
  *top_index = active_worst_quality;
  *bottom_index = active_best_quality;
  return q;
}

// Caller ignores the return value:
pick_kf_q_bound_two_pass(cpi, &active_best_quality, &active_worst_quality);
```

Good:

```cpp
static void pick_kf_q_bound_two_pass(const VP9_COMP *cpi, int *bottom_index,
                                     int *top_index) {
  ...
  *top_index = active_worst_quality;
  *bottom_index = active_best_quality;
}

// Caller:
pick_kf_q_bound_two_pass(cpi, &active_best_quality, &active_worst_quality);
```
