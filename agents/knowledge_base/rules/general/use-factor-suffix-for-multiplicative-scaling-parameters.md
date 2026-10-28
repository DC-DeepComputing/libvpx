---
name: libvpx/use-factor-suffix-for-multiplicative-scaling-parameters
description: When naming configuration parameters or variables that scale default behavior multiplicatively, use the '_factor' or '_multiplier' suffix rather than terms like '_part'.
rubric: readability
severity: should_fix
---

**Rule.** Name configuration fields, struct members, and variables that serve as
multiplicative scaling factors with the '_factor' (or '_multiplier') suffix.
Avoid terms like '_part' or ambiguous raw nouns that imply additive components
or absolute values.

**Why.** Suffixes like '_factor' or '_multiplier' clearly communicate that a
parameter represents a relative scaling multiplier applied to default behavior
rather than an absolute quantity or additive offset, preventing
misinterpretation by API consumers and maintainers.

## Examples

Bad:

```cpp
// Ambiguous '_part' implies an additive slice or absolute value rather than a multiplier.
cpi->twopass.sr_diff_part =
    (double)cfg->sr_diff_part.num / (double)cfg->sr_diff_part.den;
```

Good:

```cpp
// '_factor' explicitly indicates a multiplicative scaling factor.
cpi->twopass.sr_diff_factor =
    (double)cfg->sr_diff_factor.num / (double)cfg->sr_diff_factor.den;
```
