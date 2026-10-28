---
name: libvpx/parameterize-enum-search-range-bounds
description: When iterating over enum ranges or search modes, define boundary variables for start and end limits instead of hardcoding enum constants across loop conditions and downstream checks
rubric: readability
severity: should_fix
---

**Rule.** When searching or iterating over a range of enum options, filters, or
prediction modes whose search boundaries may vary or expand, define dedicated
boundary variables (such as filter_start and filter_end) for the range
endpoints. Use these boundary variables in loop initialization and termination
conditions as well as any in-loop or post-loop checks rather than hardcoding
specific enum constants across multiple conditional statements.

**Why.** Hardcoding literal enum constants across loop conditions and downstream
checks creates brittle coupling that easily desynchronizes when search ranges
are adjusted or new enum values are introduced. Using boundary variables keeps
loop bounds and subsequent evaluations synchronized and simplifies future search
space expansions.

## Examples

Bad:

```cpp
INTERP_FILTER filter_start = force_smooth_filter ? EIGHTTAP_SMOOTH : EIGHTTAP;
for (filter = filter_start; filter <= EIGHTTAP_SMOOTH; ++filter) {
  ...
}
if (reuse_inter_pred) {
  ...
} else if (best_filter < EIGHTTAP_SMOOTH) {
  mi->interp_filter = best_filter;
  vp9_build_inter_predictors_sby(xd, mi_row, mi_col, bsize);
}
```

Good:

```cpp
INTERP_FILTER filter_start = force_smooth_filter ? EIGHTTAP_SMOOTH : EIGHTTAP;
INTERP_FILTER filter_end = EIGHTTAP_SMOOTH;
for (filter = filter_start; filter <= filter_end; ++filter) {
  ...
}
if (reuse_inter_pred) {
  ...
} else if (best_filter < filter_end) {
  mi->interp_filter = best_filter;
  vp9_build_inter_predictors_sby(xd, mi_row, mi_col, bsize);
}
```
