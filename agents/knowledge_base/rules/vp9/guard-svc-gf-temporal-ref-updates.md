---
name: libvpx/vp9/guard-svc-gf-temporal-ref-updates
description: When updating VP9 SVC golden frame temporal reference buffers, guard updates behind use_gf_temporal_ref_current_layer, resolve buffer indices dynamically, and update layer tracking maps
rubric: correctness
severity: should_fix
---

**Rule.** In VP9 SVC reference frame buffer updates, only update golden frame
long-term temporal reference buffer slots when
svc->use_gf_temporal_ref_current_layer is enabled. Resolve the reference buffer
index dynamically via svc->buffer_gf_temporal_ref rather than hardcoding slot
numbers, and update the layer ID maps (fb_idx_spatial_layer_id and
fb_idx_temporal_layer_id) for that buffer index.

**Why.** Unconditionally updating long-term temporal reference buffer slots when
the feature is disabled inappropriately overwrites reference frame buffers and
desynchronizes layer tracking metadata across spatial and temporal layers,
leading to prediction corruption.

## Examples

Bad:

```cpp
// Update slots for golden frame long temporal prediction.
if (svc->spatial_layer_id == 1) {
  ref_cnt_fb(pool->frame_bufs, &cm->ref_frame_map[6], cm->new_fb_idx);
} else if (svc->spatial_layer_id == 2) {
  ref_cnt_fb(pool->frame_bufs, &cm->ref_frame_map[7], cm->new_fb_idx);
}
```

Good:

```cpp
if (svc->use_gf_temporal_ref_current_layer) {
  const int index = num_sl == 3 ? sl_id - 1 : sl_id;
  const int lt_buffer_index = svc->buffer_gf_temporal_ref[index].idx;
  ref_cnt_fb(pool->frame_bufs, &cm->ref_frame_map[lt_buffer_index],
             cm->new_fb_idx);
  svc->fb_idx_spatial_layer_id[lt_buffer_index] = sl_id;
  svc->fb_idx_temporal_layer_id[lt_buffer_index] = tl_id;
}
```
