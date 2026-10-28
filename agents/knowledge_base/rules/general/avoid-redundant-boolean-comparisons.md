---
name: libvpx/avoid-redundant-boolean-comparisons
description: When evaluating flags, conditions, or pointers in conditionals and logical expressions (&&, ||), rely on direct truthiness evaluation instead of comparing against == 1, == true, or using redundant double negation (!!)
rubric: readability
severity: nit
---

**Rule.** Do not explicitly compare boolean variables, struct fields (e.g.,
`allocated`, `is_key_frame`), or truthy integer conditions against `== 1`, `!=
1`, `== true`, or `== false`, and do not use redundant double negation (`!!`) on
operands within conditional statements (`if`, `while`, `for`), assertions, or
logical expressions (`||`, `&&`). Evaluate flags, pointers, and conditions
directly for truthiness (e.g., `if (flag)` or `if (ptr)`), use logical NOT for
false conditions (`if (!flag)`), or use standard explicit comparisons (such as
`ptr != NULL` or `x != 0`) when clarifying pointer or non-boolean status.

**Why.** In C and C++, conditional statements and logical operators (`&&`, `||`)
already evaluate operands in a boolean context where any non-zero value is true.
Explicit comparisons like `== 1` or `== true` and double negations like `!!` add
unnecessary visual noise, clutter expressions, and can introduce subtle bugs if
a truthy state is represented by non-zero values other than 1 (such as bitmasks
or upstream return codes).

## Examples

Bad:

```cpp
// Redundant explicit comparisons against 1 or true/false
if (cpi->use_svc && cpi->svc.spatial_layer_id > 0 &&
    cpi->svc.rc_drop_spatial_layer[cpi->svc.spatial_layer_id - 1] == 1) {
  return 1;
}

if (motion_field_info->allocated == 1) {
  vp9_free_motion_field_info(motion_field_info);
}

if (is_key_frame == true) {
  process_key_frame();
}

// Redundant double negation in logical expressions and conditions
const int have_top = loff || !!xd->above_mi;
if (!!ptr && has_data) {
  process(ptr);
}
```

Good:

```cpp
// Direct evaluation of boolean flags and struct fields
if (cpi->use_svc && cpi->svc.spatial_layer_id > 0 &&
    cpi->svc.rc_drop_spatial_layer[cpi->svc.spatial_layer_id - 1]) {
  return 1;
}

if (motion_field_info->allocated) {
  vp9_free_motion_field_info(motion_field_info);
}

if (is_key_frame) {
  process_key_frame();
}

// Clean implicit conversion or explicit comparison
const int have_top = loff || (xd->above_mi != NULL);
if (ptr != NULL && has_data) {
  process(ptr);
}
```
