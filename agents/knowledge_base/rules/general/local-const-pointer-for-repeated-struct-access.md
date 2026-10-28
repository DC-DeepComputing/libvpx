---
name: libvpx/local-const-pointer-for-repeated-struct-access
description: When repeatedly accessing fields of a nested struct or struct pointer, define a local const pointer alias rather than repeating chained dereferences or member accesses
rubric: readability
severity: nit
---

**Rule.** When repeatedly accessing fields of a nested struct member or struct
pointer (such as cpi->un_scaled_source, cpi->svc, x->e_mbd, or
stream->config.cfg) across multiple lines within a function or block, define a
local const pointer alias (e.g., const StructType *const alias = ... or
StructType *const alias = &...) at the top of the scope. Use the alias
throughout the block instead of repeating chained pointer indirection or nested
member dereferences, qualifying with const appropriately based on pointer and
pointee mutability.

**Why.** Aliasing repeated struct member chains or pointer indirection to a
local const pointer reduces visual clutter and expression length, clarifies the
active context and data mutability for readers and the compiler, and prevents
accidental pointer reassignment.

## Examples

Bad:

```cpp
// Variant 1: Chained pointer dereferences
src_y = cpi->un_scaled_source->y_buffer;
src_ystride = cpi->un_scaled_source->y_stride;
src_width = cpi->un_scaled_source->y_width;
src_height = cpi->un_scaled_source->y_height;

// Variant 2: Repeated nested struct member access
if (cpi->use_svc && cpi->svc.spatial_layer_id > 0 &&
    cpi->svc.framedrop_mode == CONSTRAINED_LAYER_DROP) {
  cpi->svc.last_layer_dropped[cpi->svc.spatial_layer_id] = 1;
}
```

Good:

```cpp
// Variant 1: Chained pointer dereferences
const YV12_BUFFER_CONFIG *const unscaled_src = cpi->un_scaled_source;
src_y = unscaled_src->y_buffer;
src_ystride = unscaled_src->y_stride;
src_width = unscaled_src->y_width;
src_height = unscaled_src->y_height;

// Variant 2: Repeated nested struct member access
SVC *const svc = &cpi->svc;
if (cpi->use_svc && svc->spatial_layer_id > 0 &&
    svc->framedrop_mode == CONSTRAINED_LAYER_DROP) {
  svc->last_layer_dropped[svc->spatial_layer_id] = 1;
}
```
