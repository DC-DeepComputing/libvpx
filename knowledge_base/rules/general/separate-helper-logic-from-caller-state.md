---
name: libvpx/separate-helper-logic-from-caller-state
description: When designing conditional helpers, evaluate feature guards in callers at triggering branches, retain caller lifecycle state in callers, and encapsulate helper-internal state resets inside helpers
rubric: design
severity: should_fix
---

**Rule.** When designing and invoking specialized helper functions (such as
`post_encode_drop_cbr`, `post_encode_drop_frame`, or
`force_refresh_longterm_ref`):
1. **Guard helper calls at caller sites**: Evaluate feature-enablement flags and preconditions (such as `rc.use_post_encode_drop` or quality thresholds) at the caller site before invoking the helper, and invoke conditional action helpers directly inside the triggering `if` branch rather than wrapping helper bodies in caller precondition checks or calling them unconditionally.
2. **Keep caller lifecycle state in callers**: Retain general encoder pipeline state updates and tracking (such as resetting `last_frame_dropped` or pipeline buffer flags) at the caller site rather than burying them inside helper fallback paths.
3. **Encapsulate helper-internal state inside helpers**: When a helper manages internal sub-state (such as `last_post_encode_dropped_scene_change`), ensure the helper encapsulates both triggering updates and fallback resets across all return paths rather than relying on callers to manage the helper's internal state flags.

**Why.** Placing caller-level precondition guards and pipeline lifecycle
tracking in caller functions maintains high function cohesion, avoids
unnecessary function call overhead when features are inactive, and prevents
helpers from being tightly coupled to specific caller contexts. Conversely,
encapsulating helper-internal state transitions and fallback resets within the
helper prevents internal state leaks, avoids cluttering call sites, and
guarantees invariant consistency regardless of where the helper is invoked.

## Examples

Bad:

```cpp
// Anti-pattern 1: Caller precondition & helper calls unconditional; caller state buried in helper
static int post_encode_drop_cbr(VP9_COMP *cpi, size_t *size) {
  if (cpi->rc.use_post_encode_drop && cm->base_qindex < cpi->rc.worst_quality) {
    if (projected_exceeds_budget(cpi)) return 1;
  }
  cpi->last_frame_dropped = 0; // Caller pipeline state leaked into helper
  return 0;
}

// Anti-pattern 2: Helper internal reset leaked to caller; action helper called unconditionally
if (post_encode_drop_cbr(cpi, size)) return;
cpi->rc.last_post_encode_dropped_scene_change = 0; // Helper internal state reset in caller

if (denoiser->denoising_level > kDenLowLow) {
  denoiser->reset = 1;
}
force_refresh_longterm_ref(cpi); // Unconditional call instead of inside triggering branch
```

Good:

```cpp
// Helper manages its own internal state and fallback resets
static int post_encode_drop_cbr(VP9_COMP *cpi, size_t *size) {
  if (projected_exceeds_budget(cpi)) {
    if (is_scene_change(cpi)) cpi->rc.last_post_encode_dropped_scene_change = 1;
    return 1;
  }
  cpi->rc.last_post_encode_dropped_scene_change = 0; // Helper encapsulates its internal reset
  return 0;
}

// Caller evaluates feature guard, manages caller state, and invokes action inside branch
if (cpi->rc.use_post_encode_drop && cm->base_qindex < cpi->rc.worst_quality) {
  if (post_encode_drop_cbr(cpi, size)) return;
}
cpi->last_frame_dropped = 0; // Caller pipeline state stays in caller

if (denoiser->denoising_level > kDenLowLow) {
  denoiser->reset = 1;
  force_refresh_longterm_ref(cpi); // Invoked directly within triggering branch
}
```
