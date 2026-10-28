---
name: libvpx/avoid-duplicate-paired-struct-field-assignments
description: When configuring or branching on paired struct fields (such as below/above or min/max), ensure both counterpart members are assigned and selected across threshold branches.
rubric: correctness
severity: should_fix
---

**Rule.** When configuring, initializing, or branching on paired or
complementary configuration struct fields (such as _below/_above, _min/_max,
_low/_high, or _left/_right):
1. Verify that all intended counterpart fields are assigned during setup and that consecutive assignments do not accidentally overwrite the same member multiple times due to copy-paste errors.
2. When branching across conditional threshold checks (such as mv_thresh or low/high limits), verify that each if/else branch selects its corresponding counterpart setting rather than duplicating the assignment across branches.

**Why.** Copy-paste errors on paired fields either leave complementary members
uninitialized or cause conditional threshold checks to apply the same setting
across both branches, silently neutralizing speed features or motion search
constraints.

## Examples

Bad:

```cpp
// 1. Accidental duplicate assignment leaves force_stop_above unset
sf->mv.adapt_subpel_force_stop.force_stop_below = 2;
sf->mv.adapt_subpel_force_stop.force_stop_below = 1;

// 2. Duplicate branch read renders threshold check ineffective
if (abs(tmp_mv->as_mv.row) <= mv_thresh ||
    abs(tmp_mv->as_mv.col) <= mv_thresh) {
  subpel_force_stop = cpi->sf.mv.adapt_subpel_force_stop.force_stop_below;
} else {
  subpel_force_stop = cpi->sf.mv.adapt_subpel_force_stop.force_stop_below;
}
```

Good:

```cpp
// 1. Both complementary counterpart fields are correctly configured
sf->mv.adapt_subpel_force_stop.force_stop_below = 2;
sf->mv.adapt_subpel_force_stop.force_stop_above = 1;

// 2. Complementary setting selected across if/else branches
if (abs(tmp_mv->as_mv.row) <= mv_thresh ||
    abs(tmp_mv->as_mv.col) <= mv_thresh) {
  subpel_force_stop = cpi->sf.mv.adapt_subpel_force_stop.force_stop_below;
} else {
  subpel_force_stop = cpi->sf.mv.adapt_subpel_force_stop.force_stop_above;
}
```
