---
name: libvpx/place-const-qualifier-before-type-name
description: When declaring const variables, pointers, or function parameters, place the const qualifier before the type name rather than after it (west const)
rubric: readability
severity: nit
---

**Rule.** Place the 'const' qualifier before the type specifier (e.g., 'const
int16_t *ptr', 'const uint8_t *buf') rather than after it (e.g., 'int16_t const
*ptr', 'uint8_t const *buf') in variable declarations, pointer declarations,
casts, and function parameter lists.

**Why.** Libvpx follows standard Google C/C++ style conventions specifying 'west
const' placement ('const Type *'). Consistent const placement maintains
uniformity across the codebase and avoids stylistic fragmentation.

## Examples

Bad:

```cpp
void vpx_hadamard_32x32_sse2(int16_t const *src_diff, ptrdiff_t src_stride,
                             tran_low_t *coeff);
```

Good:

```cpp
void vpx_hadamard_32x32_sse2(const int16_t *src_diff, ptrdiff_t src_stride,
                             tran_low_t *coeff);
```
