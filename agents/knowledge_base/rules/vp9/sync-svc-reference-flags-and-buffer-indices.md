---
name: libvpx/vp9/sync-svc-reference-flags-and-buffer-indices
description: When configuring or updating VP9 SVC reference buffers, update frame-level cpi->*_fb_idx and synchronize SVC state via vp9_copy_flags_ref_update_idx
rubric: readability
severity: should_fix
---

**Rule.** In VP9 SVC encoding, update frame-level reference indices
(cpi->lst_fb_idx, cpi->gld_fb_idx, cpi->alt_fb_idx) rather than directly
mutating persistent layer arrays (svc->*_fb_idx[sl]) during frame reference
setup. Additionally, when updating reference buffer slots or adding reference
update paths, synchronize the SVC context by calling
vp9_copy_flags_ref_update_idx(cpi) and vp9_svc_update_ref_frame_buffer_idx(cpi).

**Why.** The post-encode helper vp9_copy_flags_ref_update_idx() automatically
copies cpi->*_fb_idx into svc->*_fb_idx[sl], so mutating svc->*_fb_idx[sl]
directly mid-frame is redundant and risks state divergence. Conversely, omitting
synchronization calls after reference map modifications leaves the SVC layer
context out of sync with the encoder's buffer pool.

## Examples

Bad:

```cpp
// Redundantly mutating persistent SVC state during frame reference setup:
if (!svc->use_gf_temporal_ref) {
  svc->gld_fb_idx[sl] = svc->lst_fb_idx[sl];
  cpi->gld_fb_idx = cpi->lst_fb_idx;
}

// Missing post-update synchronization calls:
void vp9_svc_update_ref_frame_key(VP9_COMP *const cpi) {
  update_ref_frame_map(cpi);
}
```

Good:

```cpp
// Mutate frame-level cpi indices; vp9_copy_flags_ref_update_idx syncs svc:
if (!svc->use_gf_temporal_ref) {
  cpi->gld_fb_idx = cpi->lst_fb_idx;
}

// Synchronize flags and buffer index metadata after reference map update:
void vp9_svc_update_ref_frame_key(VP9_COMP *const cpi) {
  update_ref_frame_map(cpi);
  vp9_copy_flags_ref_update_idx(cpi);
  vp9_svc_update_ref_frame_buffer_idx(cpi);
}
```
