---
name: libvpx/initialize-new-struct-members-in-init-function
description: Explicitly initialize struct fields and default values inside dedicated creation, init, or reset functions rather than setting them ad-hoc across call sites
rubric: correctness
severity: should_fix
---

**Rule.** When adding or configuring struct members, feature flags, mode
toggles, or state variables on structs with dedicated lifecycle routines (such
as vp8_create_compressor(), vp9_create_compressor(), vp9_rc_init(), *_init(), or
*_reset()), explicitly initialize all fields to their intended default values
inside those centralized initialization functions (e.g., on context structs like
VP8_COMP, VP9_COMP, or RATE_CONTROL). Do not leave newly added fields
uninitialized, rely solely on implicit zero-allocation, or scatter default value
assignments ad-hoc across individual caller sites or passes; call sites should
only pass configuration parameters or override non-default values.

**Why.** In C, structs lack constructors and in-class initializers, relying
entirely on explicit creation, init, and reset routines. Centralizing default
assignments inside dedicated initialization functions documents default state,
ensures consistency across operational modes and reconfiguration paths, and
prevents bugs caused by uninitialized fields or omitted default assignments at
alternate call sites.

## Examples

Bad:

```cpp
// Variant 1: Rate control init omitting newly added struct members or setting defaults at call sites
void vp9_rc_init(const VP9EncoderConfig *oxcf, int pass, RATE_CONTROL *rc) {
  rc->force_max_q = 0;
  // Bug: last_post_encode_dropped_scene_change left uninitialized
}
// At call site:
vp9_rc_init(oxcf, 1, &rc);
rc.static_scene_max_gf_interval = 250;  // Ad-hoc default assignment at call site

// Variant 2: Compressor creation omitting newly added context flags
struct VP8_COMP *vp8_create_compressor(VP8_CONFIG *oxcf) {
  cpi->active_map_enabled = 0;
  // Bug: use_roi_static_threshold added to VP8_COMP but omitted from initialization
}
```

Good:

```cpp
// Variant 1: All fields and defaults centralized in init function
void vp9_rc_init(const VP9EncoderConfig *oxcf, int pass, RATE_CONTROL *rc) {
  rc->force_max_q = 0;
  rc->last_post_encode_dropped_scene_change = 0;
  rc->use_post_encode_drop = 0;
  rc->static_scene_max_gf_interval = 250;
}
// At call site:
vp9_rc_init(oxcf, 1, &rc);

// Variant 2: All newly added context flags explicitly initialized
struct VP8_COMP *vp8_create_compressor(VP8_CONFIG *oxcf) {
  cpi->active_map_enabled = 0;
  cpi->use_roi_static_threshold = 0;
}
```
