---
name: libvpx/cast-to-domain-typedef-rather-than-primitive
description: When casting expressions for parameters or variables with domain typedefs (like vpx_prob), cast to the typedef rather than underlying primitive types.
rubric: readability
severity: nit
---

**Rule.** When explicitly casting an expression to match a function parameter,
struct member, or variable declared with a domain typedef (such as vpx_prob,
vpx_tree_index, or tran_low_t), cast to that typedef rather than its underlying
primitive integer type (such as uint8_t or int). Avoid using raw primitive casts
that obscure domain semantics when an explicit domain typedef is available.

**Why.** Casting to domain typedefs documents the semantic intent of the data,
keeps call sites and assignments synchronized with function signatures and
struct definitions, and prevents maintenance bugs if the underlying primitive
type representation changes.

## Examples

Bad:

```cpp
// Anti-pattern: casting to raw primitive uint8_t instead of domain typedef vpx_prob
new_b = cost_branch256(ct + 2 * PIVOT_NODE, (uint8_t)newp);
update_b = prob_diff_update_cost((uint8_t)newp, oldp) + upd_cost;
bestnewp = (uint8_t)newp;
```

Good:

```cpp
// Preferred: cast to domain typedef vpx_prob
new_b = cost_branch256(ct + 2 * PIVOT_NODE, (vpx_prob)newp);
update_b = prob_diff_update_cost((vpx_prob)newp, oldp) + upd_cost;
bestnewp = (vpx_prob)newp;
```
