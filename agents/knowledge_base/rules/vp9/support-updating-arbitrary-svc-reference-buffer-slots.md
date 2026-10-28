---
name: libvpx/vp9/support-updating-arbitrary-svc-reference-buffer-slots
description: When updating or refreshing reference frame buffer slots in VP9 SVC, support updating all REF_FRAMES slots and synchronize accompanying layer tracking metadata
rubric: correctness
severity: should_fix
---

**Rule.** When implementing or updating reference frame buffer slot refresh
logic in VP9 SVC, support updating any reference buffer slot across all
`REF_FRAMES` slots rather than restricting updates to active prediction indices
(`lst_fb_idx`, `gld_fb_idx`, `alt_fb_idx`). Additionally, whenever updating or
refreshing reference frame buffer slots (`cm->ref_frame_map` via `ref_cnt_fb`),
always update the accompanying layer-tracking arrays
`svc->fb_idx_spatial_layer_id` and `svc->fb_idx_temporal_layer_id` for every
modified buffer index.

**Why.** Restricting buffer updates to active prediction references prevents
custom or flexible SVC layer configurations from updating arbitrary buffer slots
out of the 8 available VP9 frames. Failing to update the layer tracking metadata
arrays when buffer slots are refreshed leaves stale layer IDs in the buffer pool
tracking state, causing invalid layer references or assertion failures.

## Examples

Bad:

```cpp
// Bad: restricting slot refresh to active prediction indices only
for (ref = 0; ref < REF_FRAMES; ++ref) {
  if (ref == cpi->lst_fb_idx && svc->update_buffer_slot[ref][sl] == 1)
    cpi->ext_refresh_last_frame = 1;
}

// Bad: refreshing buffer slot without updating layer tracking metadata
ref_cnt_fb(pool->frame_bufs, &cm->ref_frame_map[0], cm->new_fb_idx);
```

Good:

```cpp
// Good: iterate all REF_FRAMES slots and update layer tracking metadata
for (i = 0; i < REF_FRAMES; ++i) {
  if (cm->frame_type == KEY_FRAME ||
      svc->update_buffer_slot[svc->spatial_layer_id] & (1 << i)) {
    ref_cnt_fb(pool->frame_bufs, &cm->ref_frame_map[i], cm->new_fb_idx);
    svc->fb_idx_spatial_layer_id[i] = svc->spatial_layer_id;
    svc->fb_idx_temporal_layer_id[i] = svc->temporal_layer_id;
  }
}
```
