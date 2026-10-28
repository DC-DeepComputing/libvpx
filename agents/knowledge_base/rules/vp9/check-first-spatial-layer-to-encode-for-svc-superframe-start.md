---
name: libvpx/vp9/check-first-spatial-layer-to-encode-for-svc-superframe-start
description: When checking for the start of a VP9 SVC superframe or base layer state, check first_spatial_layer_to_encode rather than hardcoding 0
rubric: correctness
severity: should_fix
---

**Rule.** In VP9 SVC encoding, check whether spatial_layer_id or
spatial_layer_to_encode equals svc->first_spatial_layer_to_encode rather than
hardcoding 0 when testing for the beginning of a superframe, performing
superframe-level initialization, or evaluating cross-layer dropping. When
testing for upper enhancement layers, compare using >
svc->first_spatial_layer_to_encode instead of > 0.

**Why.** In VP9 SVC, spatial layers may be dynamically enabled, disabled, or
configured such that encoding begins at a layer index greater than 0. Hardcoding
layer index 0 causes superframe-level initialization, rate control resets, and
frame drop checks to be skipped or evaluated incorrectly when the active base
layer is non-zero.

## Examples

Bad:

```cpp
// Skipping superframe-level initialization when layer 0 is inactive
if (svc->spatial_layer_to_encode == 0) {
  // Superframe-level initialization / cross-layer frame drop test
}
if (svc->spatial_layer_id > 0) {
  // Enhancement layer logic
}
```

Good:

```cpp
// Correctly checking against the first active spatial layer
if (svc->spatial_layer_to_encode == svc->first_spatial_layer_to_encode) {
  // Superframe-level initialization / cross-layer frame drop test
}
if (svc->spatial_layer_id > svc->first_spatial_layer_to_encode) {
  // Enhancement layer logic
}
```
