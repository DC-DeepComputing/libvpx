---
name: libvpx/match-local-variable-types-to-struct-fields
description: When declaring local variables initialized from struct fields, match their type and signedness to the member types to avoid explicit casts
rubric: readability
severity: should_fix
---

**Rule.** When declaring local variables to hold values from struct fields or
configuration members, match the variable's type and signedness to the
underlying struct field rather than introducing explicit type casts (such as
casting unsigned dimensions or bit depths to signed int). Preserve the struct
member's signedness throughout local computations unless an explicit type
conversion is functionally required.

**Why.** Matching local variable types to the source struct fields eliminates
superfluous explicit type casts and reduces visual noise. It also prevents
unintentional signed/unsigned comparison mismatches or lossy conversions across
codec interfaces.

## Examples

Bad:

```cpp
const int h = (int)img->d_h;
const int w = (int)img->d_w;
const int bit_depth = (int)ctx->oxcf.input_bit_depth;
```

Good:

```cpp
const unsigned int h = img->d_h;
const unsigned int w = img->d_w;
const unsigned int bit_depth = ctx->oxcf.input_bit_depth;
```
