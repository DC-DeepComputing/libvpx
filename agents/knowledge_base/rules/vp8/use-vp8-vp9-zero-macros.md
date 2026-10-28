---
name: libvpx/vp8/use-vp8-vp9-zero-macros
description: In VP8 and VP9 code, use vp8_zero, vp9_zero, or their _array helpers instead of manual memset, for loops, chained assignments, or dereferenced pointers
rubric: readability
severity: nit
---

**Rule.** In VP8 and VP9 code, use the codec zeroing helper macros (`vp8_zero`,
`vp9_zero`) for structs, fixed-size variables, and arrays, and use array helpers
(`vp8_zero_array`, `vp9_zero_array`) when clearing array buffers with an
explicit element count. Do not write open-coded `memset(..., 0, sizeof(...))`
calls, manual `for` loops zeroing elements, or chained scalar assignments (e.g.,
`arr[0] = arr[1] = ... = 0;`). Additionally, never pass a dereferenced pointer
(`*ptr`) to `vp8_zero` or `vp9_zero`, as `sizeof(*ptr)` evaluates to a single
element rather than the full buffer; pass the pointer and length to
`vp8_zero_array` or `vp9_zero_array` instead.

**Why.** Subsystem zeroing macros (`vp8_zero`, `vp9_zero`, `vp8_zero_array`,
`vp9_zero_array`) eliminate repetitive `memset` and `sizeof` boilerplate,
eliminate indexing logic and loops, and adhere to libvpx codebase idioms. They
prevent size calculation mismatches and address-of errors. Calling
`vp8_zero(*ptr)` or `vp9_zero(*ptr)` on a pointer is a common bug because
`sizeof(*ptr)` zeroes only one element rather than the entire buffer, leaving
remaining elements uninitialized.

## Examples

Bad:

```cpp
// Manual memset on structs or variables
memset(&ctx->oxcf, 0, sizeof(ctx->oxcf));
memset(mode_mv_sb, 0, sizeof(mode_mv_sb));

// Manual for-loop or chained assignments to zero an array
for (i = 0; i < cpi->svc.number_spatial_layers; i++) {
  cpi->svc.drop_spatial_layer[i] = 0;
}
map[0] = map[1] = map[2] = map[3] = 0;

// Bug: vp8_zero on dereferenced pointer only zeroes 1 element
vp8_zero(*map_ptr);
```

Good:

```cpp
// Zero structs and fixed-size variables
vp8_zero(ctx->oxcf);
vp8_zero(mode_mv_sb);

// Zero fixed-size arrays or collections
vp9_zero(cpi->svc.drop_spatial_layer);
vp8_zero_array(map, 4);

// Zero dynamically sized or pointer-backed array buffers
vp8_zero_array(map_ptr, num_elements);
```
