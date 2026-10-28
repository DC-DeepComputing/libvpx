---
name: libvpx/extract-repeated-subexpressions-to-local-const
description: When computing identical non-trivial arithmetic subexpressions across multiple assignments or branches, extract the calculation into a named local const variable
rubric: readability
severity: nit
---

**Rule.** Extract repeated identical arithmetic subexpressions or multi-step
calculations within a function body into a named local const variable. Reference
the named local constant across subsequent formulas, assignments, and branches
instead of recomputing the same subexpression multiple times.

**Why.** Duplicating non-trivial arithmetic calculations across multiple
statements obscures intent, invites copy-paste discrepancies during maintenance,
and forces redundant recalculation.

## Examples

Bad:

```cpp
qzbin_factor = 64 + 16 * (7 - cpi->oxcf.sharpness) / 7;
qrounding_factor = 64 - 16 * (7 - cpi->oxcf.sharpness) / 7;
// ...
qrounding_factor_fp = 64 - 16 * (7 - cpi->oxcf.sharpness) / 7;
```

Good:

```cpp
const int sharpness_adjustment = 16 * (7 - cpi->oxcf.sharpness) / 7;

qzbin_factor = 64 + sharpness_adjustment;
qrounding_factor = 64 - sharpness_adjustment;
// ...
qrounding_factor_fp = 64 - sharpness_adjustment;
```
