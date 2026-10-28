---
name: libvpx/consolidate-shared-preconditions-under-single-if
description: When multiple consecutive statements or loops share an identical precondition, group them inside a single enclosing if statement rather than repeating the check
rubric: readability
severity: should_fix
---

**Rule.** When multiple consecutive statements, inner conditionals, or loops
share an identical precondition, group them inside a single enclosing if block.
Do not repeat the shared condition across each individual statement guard or
loop header condition.

**Why.** Consolidating shared preconditions into a single outer block removes
duplicated check expressions, avoids re-evaluating loop header conditions on
each iteration, and improves readability.

## Examples

Bad:

```cpp
for (t = TX_4X4; t <= TX_32X32 && !cm->show_existing_frame; ++t)
  full_to_model_counts(cpi->td.counts->coef[t], cpi->td.rd_counts.coef_counts[t]);

if (!cm->error_resilient_mode && !cm->show_existing_frame) {
  vp9_adapt_coef_probs(cm);
}
```

Good:

```cpp
if (!cm->show_existing_frame) {
  for (t = TX_4X4; t <= TX_32X32; ++t) {
    full_to_model_counts(cpi->td.counts->coef[t], cpi->td.rd_counts.coef_counts[t]);
  }

  if (!cm->error_resilient_mode) {
    vp9_adapt_coef_probs(cm);
  }
}
```
