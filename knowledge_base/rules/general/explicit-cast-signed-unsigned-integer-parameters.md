---
name: libvpx/explicit-cast-signed-unsigned-integer-parameters
description: When passing unsigned integer expressions to signed function parameters or converting between signedness, use an explicit cast to prevent implicit conversion warnings
rubric: readability
severity: should_fix
---

**Rule.** When passing unsigned integer expressions (such as `unsigned int`,
`uint32_t`, or `size_t`) to function parameters or struct fields expecting a
signed `int` (or vice versa), use an explicit cast (preferring
`static_cast<int>` in C++ files and `(int)` in C files) when the types cannot be
matched at declaration. Avoid implicit signed/unsigned conversions across
function calls and assignments.

**Why.** Implicit conversions between signed and unsigned integer types can lead
to unintended value wrap-around, silent data truncation, or compiler diagnostic
warnings across different platforms and toolchains. Explicit casting clarifies
the developer's intent and ensures warning-free compilation under strict
signedness flags.

## Examples

Bad:

```cpp
// In C++ RTC rate control wrapper:
const unsigned int prev_number_of_layers = oxcf->number_of_layers;
vp8_reset_temporal_layer_change(cpi_, oxcf, prev_number_of_layers);
```

Good:

```cpp
// In C++ RTC rate control wrapper:
const unsigned int prev_number_of_layers = oxcf->number_of_layers;
vp8_reset_temporal_layer_change(cpi_, oxcf,
                                static_cast<int>(prev_number_of_layers));
```
