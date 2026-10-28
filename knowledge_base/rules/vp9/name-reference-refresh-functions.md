---
name: libvpx/vp9/name-reference-refresh-functions
description: When naming functions that trigger or configure reference frame buffer updates, use refresh_... or force_refresh_... instead of set_reference_... prefixes.
rubric: readability
severity: nit
---

**Rule.** Name functions that configure or trigger reference frame buffer
updates with refresh_... or force_refresh_... prefixes rather than
set_reference_.... Reserve set_reference_... for selecting reference frames used
for prediction to avoid confusing buffer updates with reference configuration.

**Why.** In VP8 and VP9 codecs, setting flags like refresh_alt_ref_frame causes
the current encoded frame to overwrite a reference buffer slot. Using refresh or
force_refresh accurately reflects this buffer modification side effect, whereas
set_reference misleadingly implies choosing which buffer to read or predict
from.

## Examples

Bad:

```cpp
static void set_reference_long_term_temporal(VP9_COMP *const cpi) {
  SVC *const svc = &cpi->svc;
  if (svc->use_gf_temporal_ref_current_layer) {
    int index = svc->spatial_layer_id;
    if (svc->number_spatial_layers == 3) index = svc->spatial_layer_id - 1;
    assert(index >= 0);
    cpi->alt_fb_idx = svc->buffer_gf_temporal_ref[index].idx;
    cpi->refresh_alt_ref_frame = 1;
  }
}
```

Good:

```cpp
static void force_refresh_longterm_ref(VP9_COMP *const cpi) {
  SVC *const svc = &cpi->svc;
  if (svc->use_gf_temporal_ref_current_layer) {
    int index = svc->spatial_layer_id;
    if (svc->number_spatial_layers == 3) index = svc->spatial_layer_id - 1;
    assert(index >= 0);
    cpi->alt_fb_idx = svc->buffer_gf_temporal_ref[index].idx;
    cpi->refresh_alt_ref_frame = 1;
  }
}
```
