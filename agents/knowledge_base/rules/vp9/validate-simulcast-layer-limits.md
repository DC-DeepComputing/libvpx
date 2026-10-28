---
name: libvpx/vp9/validate-simulcast-layer-limits
description: When enabling VP9 SVC simulcast mode, verify multiple spatial layers are configured (1 < number_spatial_layers <= 3) and temporal layers do not exceed 3
rubric: correctness
severity: should_fix
---

**Rule.** When activating VP9 SVC simulcast mode (with inter-layer prediction
disabled via disable_inter_layer_pred == INTER_LAYER_PRED_OFF), ensure multiple
spatial layers are configured (svc->number_spatial_layers > 1) and verify that
neither spatial nor temporal layer counts exceed 3 (svc->number_spatial_layers
<= 3 && svc->number_temporal_layers <= 3). Do not enable simulcast mode or add
simulcast-specific handling for single spatial layer streams
(number_spatial_layers <= 1) or configurations exceeding 3 spatial or temporal
layers; keep simulcast_mode = 0 in those cases.

**Why.** Simulcast mode alters reference buffer slot management, rate control,
and speed features specifically for encoding multiple independent spatial
streams without inter-layer prediction. Single spatial layer streams
(number_spatial_layers <= 1) represent standard single-layer encoding and must
not activate simulcast mode or execute simulcast-specific buffer slot logic.
Furthermore, VP9's fixed pool of 8 reference frame buffer slots (REF_FRAMES)
cannot accommodate more than 3 spatial and 3 temporal layers without slot
overflow.

## Examples

Bad:

```cpp
// Bad: Enabling simulcast unconditionally without checking layer limits
if (svc->disable_inter_layer_pred == INTER_LAYER_PRED_OFF &&
    svc->number_spatial_layers <= 3 &&
    !(svc->temporal_layering_mode == VP9E_TEMPORAL_LAYERING_MODE_BYPASS &&
      svc->use_set_ref_frame_config))
  svc->simulcast_mode = 1;

// Bad: Handling single spatial layer within simulcast logic
if (svc->simulcast_mode && cpi->common.frame_type == KEY_FRAME) {
  if (svc->number_spatial_layers == 1) {
    svc->update_buffer_slot[sl] = 3;
  }
}
```

Good:

```cpp
// Good: Require 1 < number_spatial_layers <= 3 and number_temporal_layers <= 3
if (svc->disable_inter_layer_pred == INTER_LAYER_PRED_OFF &&
    svc->number_spatial_layers > 1 && svc->number_spatial_layers <= 3 &&
    svc->number_temporal_layers <= 3 &&
    !(svc->temporal_layering_mode == VP9E_TEMPORAL_LAYERING_MODE_BYPASS &&
      svc->use_set_ref_frame_config)) {
  svc->simulcast_mode = 1;
} else {
  svc->simulcast_mode = 0;
}
```
