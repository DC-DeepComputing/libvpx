---
name: libvpx/combine-adjacent-uninitialized-declarations
description: When declaring consecutive uninitialized local variables of the same type (scalar or SIMD) in C functions, combine them into a single comma-separated declaration
rubric: readability
severity: nit
---

**Rule.** When declaring consecutive uninitialized local variables or registers
of the same type (such as primitive scalars int, unsigned int, size_t, libvpx
typedefs tran_low_t, vpx_prob, or SIMD vector types __m128i, __m256i,
uint8x16_t) in C functions or block scopes, combine them into a single
comma-separated declaration statement (e.g., int clamp, i; or __m128i filter,
work;). Do not declare adjacent uninitialized variables of the same type across
separate single-variable lines. Avoid combining declarations across different
types, mixing pointer declarations with value declarations, or mixing
initialized with uninitialized variables in the same statement.

**Why.** Combining adjacent uninitialized declarations of the same type reduces
vertical clutter and boilerplate in function prologues and block scopes, groups
related temporary registers and scratch variables together, and conforms to
libvpx C coding conventions.

## Examples

Bad:

```cpp
void postproc_filter(void) {
  int clamp;
  int i;
  int row;
  int col;
  unsigned int count;
  unsigned int stride;
  __m128i filter;
  __m128i work;
  /* ... */
}
```

Good:

```cpp
void postproc_filter(void) {
  int clamp, i, row, col;
  unsigned int count, stride;
  __m128i filter, work;
  /* ... */
}
```
