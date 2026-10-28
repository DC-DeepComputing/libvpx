---
name: libvpx/no-redundant-void-pointer-casts
description: When passing pointers to void* or const void* parameters or assigning void* allocation return values in C, omit redundant explicit pointer casts
rubric: readability
severity: nit
---

**Rule.** In C source and header files, do not explicitly cast pointers when
passing arguments to functions accepting void * or const void * (such as
memcpy(), memmove(), or memset()), nor when assigning void * return values from
memory allocation functions (such as vpx_calloc(), vpx_malloc(), vpx_memalign(),
malloc(), or calloc()), including inside CHECK_MEM_ERROR(). Rely on C's implicit
conversions between void * and any object pointer type.

**Why.** In C, conversions between void * (or const void *) and object pointer
types are automatic and bidirectional. Explicit casts between void * and object
pointers are redundant, clutter call sites and allocation statements, and can
mask unintended type mismatches or accidentally discard const qualifiers.

## Examples

Bad:

```cpp
// Redundant casts when passing pointer arguments to (const) void* parameters
idct16x16_256_add_half1d((const int16_t *)input, row_idct_output, dest, stride, 1);
memcpy(dst_buf, (const void *)src_buf, byte_count);

// Redundant casts on void* allocation return values in C
CHECK_MEM_ERROR(cm, row_mt_handle->recon_map,
                (int8_t *)vpx_calloc(num_sbs, sizeof(*row_mt_handle->recon_map)));
tile_data = (TileData *)vpx_malloc(num_tiles * sizeof(*tile_data));
```

Good:

```cpp
// Implicit conversion to (const) void* without explicit casts
idct16x16_256_add_half1d(input, row_idct_output, dest, stride, 1);
memcpy(dst_buf, src_buf, byte_count);

// Implicit conversion from void* allocation return values in C
CHECK_MEM_ERROR(cm, row_mt_handle->recon_map,
                vpx_calloc(num_sbs, sizeof(*row_mt_handle->recon_map)));
tile_data = vpx_malloc(num_tiles * sizeof(*tile_data));
```
