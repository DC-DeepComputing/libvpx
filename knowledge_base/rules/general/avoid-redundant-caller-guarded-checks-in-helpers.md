---
name: libvpx/avoid-redundant-caller-guarded-checks-in-helpers
description: When implementing internal or feature-specific helper functions, avoid redundant condition or mode-flag checks already verified by callers; use assertions instead
rubric: readability
severity: nit
---

**Rule.** Do not add redundant runtime condition checks or mode/feature-flag
checks (such as `use_svc`, `noise_sensitivity > 0`, or `simulcast_mode`) inside
dedicated or internal helper functions when all caller call sites already verify
those conditions prior to invocation. Rely on caller preconditions, keep
dedicated helpers focused on their specific feature logic, and use assertions
(such as `assert()`) if verification of preconditions is desired.

**Why.** Duplicating caller-verified conditions inside internal helpers
introduces unnecessary branching depth, dead branches, and visual clutter. When
a helper is dedicated to a specific mode or feature, gating inside the helper
obscures module invariants and creates maintenance overhead.

## Examples

Bad:

```cpp
// Caller guarantees precondition:
if (cpi->svc.simulcast_mode && is_one_pass_cbr_svc(cpi)) {
  vp9_svc_update_ref_frame_key(cpi);
}
// Callee redundantly re-checks caller precondition:
void vp9_svc_update_ref_frame_key(VP9_COMP *const cpi) {
  if (cpi->svc.simulcast_mode) {
    if (cpi->svc.spatial_layer_id == 0) update_key_frame_context(cpi);
  }
}
// Feature helper redundantly checking feature flag:
int vp9_denoise_svc_non_key(VP9_COMP *const cpi) {
  if (cpi->use_svc) {
    return denoise_svc(cpi) && !cpi->svc.layer_context[0].is_key_frame;
  }
  return 0;
}
```

Good:

```cpp
// Caller guarantees precondition:
if (cpi->svc.simulcast_mode && is_one_pass_cbr_svc(cpi)) {
  vp9_svc_update_ref_frame_key(cpi);
}
// Callee asserts precondition or directly executes logic:
void vp9_svc_update_ref_frame_key(VP9_COMP *const cpi) {
  assert(cpi->svc.simulcast_mode);
  if (cpi->svc.spatial_layer_id == 0) update_key_frame_context(cpi);
}
// Feature helper directly executes assuming caller verified use_svc:
int vp9_denoise_svc_non_key(VP9_COMP *const cpi) {
  return denoise_svc(cpi) && !cpi->svc.layer_context[0].is_key_frame;
}
```
