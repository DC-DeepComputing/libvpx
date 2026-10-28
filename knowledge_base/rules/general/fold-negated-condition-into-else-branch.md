---
name: libvpx/fold-negated-condition-into-else-branch
description: When running logic under the logical negation of an if condition, fold it into the existing else branch instead of adding a separate redundant if check
rubric: readability
severity: nit
---

**Rule.** When logic executes only when a preceding if condition evaluates to
false, place the code inside the existing else block instead of creating a
separate subsequent if statement with the negated condition. Avoid re-evaluating
inverted boolean expressions (such as !A && !B following if (A || B)) across
consecutive conditional blocks.

**Why.** Evaluating the exact logical negation of an earlier branch condition in
a separate if statement is redundant, risks state desynchronization if
intermediate variables change, and obscures control flow by separating mutually
exclusive execution paths.

## Examples

Bad:

```cpp
if (frame_is_intra_only(cm) || boost_frame) {
  // Intra or boosted frame handling
} else {
  // Non-boosted inter frame handling
}

if (!frame_is_intra_only(cm) && !boost_frame) {
  active_best_quality = VPXMAX(active_best_quality, rc->last_boosted_qindex);
}
```

Good:

```cpp
if (frame_is_intra_only(cm) || boost_frame) {
  // Intra or boosted frame handling
} else {
  // Non-boosted inter frame handling
  active_best_quality = VPXMAX(active_best_quality, rc->last_boosted_qindex);
}
```
