---
name: libvpx/document-non-obvious-formulas-and-heuristics
description: When implementing non-obvious mathematical formulas, interpolation heuristics, or tuning calculations, precede them with an explanatory comment detailing their rationale and derivation
rubric: readability
severity: nit
---

**Rule.** When implementing non-obvious mathematical formulas, interpolation
heuristics, rate-control calculations, or quality tuning equations, precede them
with an explanatory comment documenting the underlying derivation, mathematical
intent, or heuristic rationale. Do not rely on complex multi-term arithmetic
alone to communicate the algorithm's purpose to future maintainers.

**Why.** Complex arithmetic expressions, weighted averages, and heuristic
scaling formulas are difficult to deduce or verify from raw code alone.
Explanatory comments clarify the derivation and intended behavior, making it
easier to review, debug, and tune algorithmic parameters without introducing
regressions.

## Examples

Bad:

```cpp
if (gf_group->rf_level[gf_group_index] == GF_ARF_LOW) {
  const int layer_depth = gf_group->layer_depth[gf_group_index];
  active_best_quality = ((layer_depth - 1) * cq_level +
                         active_best_quality + layer_depth / 2) /
                        layer_depth;
}
```

Good:

```cpp
if (gf_group->rf_level[gf_group_index] == GF_ARF_LOW) {
  const int layer_depth = gf_group->layer_depth[gf_group_index];
  // Linearly fit the frame q depending on the layer depth index from
  // the base layer ARF.
  active_best_quality = ((layer_depth - 1) * cq_level +
                         active_best_quality + layer_depth / 2) /
                        layer_depth;
}
```
