---
name: libvpx/vp9/include-all-reference-slots-in-svc-update-masks
description: When configuring update_buffer_slot masks for VP9 SVC or simulcast layers, include all primary and temporal slots and express masks as explicit powers-of-two with comments
rubric: correctness
severity: should_fix
---

**Rule.** When configuring reference buffer update masks (`update_buffer_slot`)
for VP9 SVC or simulcast mode (such as during key frame initialization or layer
updates), ensure all required reference buffer slots for each spatial
layer—including primary slots and secondary/auxiliary slots (e.g., slot 6 for
spatial layer 1 and slot 7 for spatial layer 2 in 3-spatial-layer simulcast, or
long-term temporal reference slots when `use_gf_temporal_ref_current_layer` is
enabled)—are included. Express bitmasks explicitly as bitwise ORs or sums of
powers-of-two (e.g., `(1 << 0) | (1 << 3)` or `1 + 4`) accompanied by inline
comments documenting target slot indices, rather than using opaque decimal
literals.

**Why.** Opaque decimal literals obscure target buffer slots and make it easy to
accidentally omit required buffer slots during layer configuration. In SVC
simulcast mode or when temporal references are enabled, omitting assigned buffer
slots (such as slot 6/7 or long-term temporal references) leaves frame buffers
unrefreshed and desynchronizes internal frame buffer state from exported
reference configuration APIs and bitstream headers, causing stale reference
frames and prediction corruption across layers.

## Examples

Bad:

```cpp
// Bad: Opaque decimal literals and omitted temporal reference slots (slots 6 & 7).
if (svc->simulcast_mode && cpi->common.frame_type == KEY_FRAME) {
  if (svc->number_spatial_layers == 3) {
    if (sl == 0) svc->update_buffer_slot[sl] = 9;   // opaque literal (slots 0, 3)
    if (sl == 1) svc->update_buffer_slot[sl] = 18;  // misses slot 6 (slots 1, 4)
    if (sl == 2) svc->update_buffer_slot[sl] = 36;  // misses slot 7 (slots 2, 5)
  }
}

// Bad: Omitting conditional long-term temporal reference slot from update mask.
if (svc->number_spatial_layers == 2) {
  if (sl == 0) svc->update_buffer_slot[sl] = 1 + 4;
  else if (sl == 1) svc->update_buffer_slot[sl] = 2 + 8;
  // Missing use_gf_temporal_ref_current_layer check
}
```

Good:

```cpp
// Good: Explicit bitwise ORs with comments, including all primary and temporal slots.
if (svc->simulcast_mode && cpi->common.frame_type == KEY_FRAME) {
  if (svc->number_spatial_layers == 3) {
    if (sl == 0) svc->update_buffer_slot[sl] = (1 << 0) | (1 << 3);              // slots 0, 3
    if (sl == 1) svc->update_buffer_slot[sl] = (1 << 1) | (1 << 4) | (1 << 6);  // slots 1, 4, 6
    if (sl == 2) svc->update_buffer_slot[sl] = (1 << 2) | (1 << 5) | (1 << 7);  // slots 2, 5, 7
  }
}

// Good: Explicitly adding conditional long-term temporal reference slot to mask.
if (svc->use_gf_temporal_ref_current_layer) {
  const int index = svc->number_spatial_layers == 3 ? sl - 1 : sl;
  const int ltf = svc->buffer_gf_temporal_ref[index].idx;
  svc->update_buffer_slot[sl] |= (1 << ltf);  // slot ltf (temporal ref)
}
```
