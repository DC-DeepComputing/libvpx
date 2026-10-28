---
name: libvpx/vp9/check-zero-initialized-dimensions-with-truthiness
description: When checking whether zero-initialized encoder dimension fields (such as initial_width or initial_height) are set, evaluate boolean truthiness or != 0 instead of > 0
rubric: readability
severity: nit
---

**Rule.** When checking whether zero-initialized encoder dimension fields (such
as `cpi->initial_width` and `cpi->initial_height`) have been configured or set,
evaluate them using boolean truthiness (`if (cpi->initial_width)` / `if
(!cpi->initial_width)`) or compare against `!= 0` / `== 0`. Do not use
relational inequality comparisons such as `> 0` to test whether dimension fields
are set.

**Why.** Encoder dimension fields use 0 as a sentinel value representing an
unconfigured or unset resolution. Testing whether a zero-initialized dimension
is set with `> 0` implies negative dimensions are possible, whereas boolean
truthiness or equality comparisons accurately reflect the sentinel state and
align with encoder codebase conventions.

## Examples

Bad:

```cpp
if (cpi->svc.number_spatial_layers > 1 && cpi->initial_width > 0 &&
    cpi->initial_height > 0) {
  // ...
}
```

Good:

```cpp
if (cpi->svc.number_spatial_layers > 1 && cpi->initial_width &&
    cpi->initial_height) {
  // ...
}
```
