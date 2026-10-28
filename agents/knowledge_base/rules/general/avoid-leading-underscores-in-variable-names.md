---
name: libvpx/avoid-leading-underscores-in-variable-names
description: When declaring local variables or function parameters, avoid leading underscores for identifier disambiguation; choose descriptive, self-explanatory variable names instead.
rubric: readability
severity: nit
---

**Rule.** Do not use leading underscores (such as `_var`) when naming local
variables, function parameters, or internal constants. To disambiguate an
identifier from an existing parameter or type, choose a distinct, descriptive
name (such as `bitdepth_max` or `bit_depth`) rather than prefixing with an
underscore.

**Why.** In C and C++, identifiers with leading underscores are non-idiomatic
and can conflict with implementation-reserved symbols or compiler intrinsics.
Using clear, descriptive names makes the code easier to read and maintain.

## Examples

Bad:

```cpp
void process_block(int bd) {
  const int _bd = (1 << bd) - 1;
  // ...
}
```

Good:

```cpp
void process_block(int bd) {
  const int bitdepth_max = (1 << bd) - 1;
  // ...
}
```
