---
name: libvpx/vp9/differentiate-layer-depth-for-gf-vs-overlay-update
description: When configuring VP9 GF group frame hierarchies, set layer_depth by update type (0 for GF_UPDATE, MAX_ARF_LAYERS - 1 for OVERLAY_UPDATE) and maintain max_layer_depth consistently
rubric: correctness
severity: should_fix
---

**Rule.** When defining VP9 GOP structure parameters in a GF_GROUP, assign
layer_depth = 0 for Golden Frame base updates (GF_UPDATE) and leaf layer depth
layer_depth = MAX_ARF_LAYERS - 1 for alternate reference frame (ARF) overlay
updates (OVERLAY_UPDATE), rather than unconditionally assigning 0 or maximum
layer depth. Additionally, ensure gf_group->max_layer_depth consistently
reflects the highest layer depth assigned to any frame in the group (e.g.,
MAX_ARF_LAYERS - 1 when overlay frames are present).

**Why.** In VP9 multi-layer ARF GOP hierarchies, Golden Frame updates serve as
base temporal layer reference frames (layer_depth = 0), whereas ARF overlay
frames are leaf-level inter frames displayed over the previous alt-ref frame
(layer_depth = MAX_ARF_LAYERS - 1). Misclassifying overlay frames as base layer
or setting an inconsistent gf_group->max_layer_depth skews frame context
indexing, corrupts rate-control layer bit allocations and qindex calculations,
and triggers assertion failures in downstream rate control.

## Examples

Bad:

```cpp
// Setting wrong layer_depth for GF vs overlay updates:
if (rc->source_alt_ref_active) {
  gf_group->update_type[frame_index] = OVERLAY_UPDATE;
  gf_group->rf_level[frame_index] = INTER_NORMAL;
} else {
  gf_group->update_type[frame_index] = GF_UPDATE;
  gf_group->rf_level[frame_index] = GF_ARF_STD;
}
gf_group->layer_depth[frame_index] = 0;  // or unconditionally MAX_ARF_LAYERS - 1

// Setting max_layer_depth inconsistent with overlay frames:
gf_group_set_overlay_frame(gf_group, 0);
gf_group->max_layer_depth = 2;
```

Good:

```cpp
// Differentiating layer_depth by update type:
if (rc->source_alt_ref_active) {
  gf_group->update_type[frame_index] = OVERLAY_UPDATE;
  gf_group->rf_level[frame_index] = INTER_NORMAL;
  gf_group->layer_depth[frame_index] = MAX_ARF_LAYERS - 1;
} else {
  gf_group->update_type[frame_index] = GF_UPDATE;
  gf_group->rf_level[frame_index] = GF_ARF_STD;
  gf_group->layer_depth[frame_index] = 0;
}

// Setting max_layer_depth to match overlay frames:
gf_group_set_overlay_frame(gf_group, 0);
gf_group->max_layer_depth = MAX_ARF_LAYERS - 1;
```
