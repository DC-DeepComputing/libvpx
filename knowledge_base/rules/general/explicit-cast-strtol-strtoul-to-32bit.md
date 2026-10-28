---
name: libvpx/explicit-cast-strtol-strtoul-to-32bit
description: When assigning return values from `strtoul()` or `strtol()` to 32-bit integer types like `uint32_t` or `int32_t`, add an explicit cast to prevent narrowing warnings
rubric: readability
severity: should_fix
---

**Rule.** When assigning the result of `strtoul()`, `strtol()`, `strtoull()`, or
`strtoll()` to fixed-width 32-bit integer types (such as `uint32_t`, `int32_t`,
`unsigned int`, or `int`), explicitly cast the return value to the target type.

**Why.** `strtoul()` and `strtol()` return `unsigned long` and `long`, which are
64 bits wide on LP64 platforms. Assigning them directly to 32-bit integer
variables causes implicit narrowing conversions that trigger compiler warnings
on 64-bit builds.

## Examples

Bad:

```cpp
uint32_t rawval;
rawval = strtoul(arg->val, &endptr, 10);

int32_t signed_val;
signed_val = strtol(arg->val, &endptr, 10);
```

Good:

```cpp
uint32_t rawval;
rawval = (uint32_t)strtoul(arg->val, &endptr, 10);

int32_t signed_val;
signed_val = (int32_t)strtol(arg->val, &endptr, 10);
```
