---
name: libvpx/separate-mode-flags-from-state-booleans
description: When creating boolean helper variables named for a specific state, do not bury mode flags inside them; check mode flags explicitly at decision sites
rubric: readability
severity: should_fix
---

**Rule.** Do not embed configuration, feature, or mode flags into boolean helper
variables named after a specific state or condition. Keep boolean helper
variables strictly reflective of the state described in their identifier, and
combine them with mode or configuration checks explicitly at the decision site.

**Why.** Baking a mode or configuration check into a state-named variable
obscures whether the underlying condition actually occurred or whether the
feature was disabled, leading to misleading variable semantics and confusing
downstream logic.

## Examples

Bad:

```cpp
// Mode flag embedded in state-named helper variable:
if (cpi->use_svc && cpi->svc.framedrop_mode &&
    cpi->svc.spatial_layer_id > 0 &&
    cpi->svc.drop_spatial_layer[cpi->svc.spatial_layer_id - 1]) {
  svc_prev_layer_dropped = 1;
}
if (svc_prev_layer_dropped || vp9_rc_drop_frame(cpi)) {
  // ...
}
```

Good:

```cpp
// Helper variable reflects only the state; mode checked at decision site:
if (cpi->use_svc && cpi->svc.spatial_layer_id > 0 &&
    cpi->svc.drop_spatial_layer[cpi->svc.spatial_layer_id - 1]) {
  svc_prev_layer_dropped = 1;
}
if ((svc_prev_layer_dropped && cpi->svc.framedrop_mode) ||
    vp9_rc_drop_frame(cpi)) {
  // ...
}
```
