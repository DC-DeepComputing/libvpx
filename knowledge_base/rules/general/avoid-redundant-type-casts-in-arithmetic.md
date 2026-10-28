---
name: libvpx/avoid-redundant-type-casts-in-arithmetic
description: When promoting binary arithmetic expressions to wider integer types, avoid redundant type casts across multiple operands when casting one operand suffices
rubric: readability
severity: nit
---

**Rule.** Avoid redundant type casts in binary arithmetic expressions when
casting a single operand (or an operand's existing type) is already sufficient
to promote the entire operation to the required width. Do not cast every operand
or subexpression to the same target type unless explicitly required to prevent
intermediate overflow.

**Why.** In C and C++, binary operators automatically promote both operands to
the common wider type via usual arithmetic conversions. Explicitly casting
multiple operands or already-promoted subexpressions to the same type adds
visual clutter and obscures the core arithmetic logic.

## Examples

Bad:

```cpp
const uint64_t duration_us =
    (uint64_t)duration * (uint64_t)ctx->timestamp_ratio.num /
    ((uint64_t)ctx->timestamp_ratio.den * (TICKS_PER_SEC / 1000000));
```

Good:

```cpp
const uint64_t duration_us =
    duration * (uint64_t)ctx->timestamp_ratio.num /
    (ctx->timestamp_ratio.den * (TICKS_PER_SEC / 1000000));
```
