---
name: libvpx/annotate-temporary-workarounds-with-todo
description: When introducing temporary workarounds, feature disablements, transitional parameters, hardcoded defaults, or deferred optimizations, annotate with attributed TODO(owner_or_bug) comments, and remove or update obsolete comments when resolved
rubric: readability
severity: should_fix
---

**Rule.** When introducing temporary workarounds, feature disablements, fallback
paths, stubbed early returns, transitional parameters or macro arguments,
hardcoded defaults (such as fixed bit depths), or deferred optimizations and
cleanups, annotate the code with an actionable, attributed `TODO(username)` or
`TODO(bug_id)` detailing the required follow-up task and triggering conditions.
Conversely, when resolving, relaxing, or modifying those constraints (such as
re-enabling restricted modes or completing deferred paths), remove or update
surrounding comments and `TODO`s that contradict the updated behavior.

**Why.** Temporary workarounds, transitional parameters, hardcoded defaults, and
deferred optimizations easily get forgotten across development cycles,
accumulating as permanent technical debt without explicit ownership and
tracking. Attributed `TODO` comments ensure clear ownership, context, and
removal milestones. Additionally, retaining obsolete comments or TODOs after
relaxing constraints contradicts code semantics and confuses maintainers.

## Examples

Bad:

```cpp
// 1. Untracked workaround / temporary feature disablement:
if (cpi->use_svc && cpi->oxcf.drop_frames_water_mark)
  sf->adaptive_rd_thresh_row_mt = 0;

// 2. Transitional macro argument or parameter without cleanup trigger:
#define FUN_CONV_1D(name, offset, step_q4, dir, src_start, avg, opt, is_avg) \
  void vpx_convolve8_##name##_##opt(uint8_t *src, ...)

// 3. Hardcoded default in incomplete routine / stubbed early return:
return vp9_convert_q_to_qindex(q_step, VPX_BITS_8);

// 4. Deferred optimization / unoptimized polling loop without tracking:
while (!*map) {
  sched_yield();
}
```

Good:

```cpp
// 1. Annotated workaround with actionable TODO:
// TODO(marpan/jianj): Look into test failure with SVC and re-enable.
if (cpi->use_svc && cpi->oxcf.drop_frames_water_mark)
  sf->adaptive_rd_thresh_row_mt = 0;

// 2. Attributed TODO documenting transitional parameter cleanup trigger:
// TODO(username@google.com): Remove is_avg once 4-tap vert avg filter exists.
#define FUN_CONV_1D(name, offset, step_q4, dir, src_start, avg, opt, is_avg) \
  void vpx_convolve8_##name##_##opt(uint8_t *src, ...)

// 3. Attributed TODO tracking high bit-depth support:
// TODO(angiebird): Support high bit depth instead of hardcoding 8-bit.
return vp9_convert_q_to_qindex(q_step, VPX_BITS_8);

// 4. Tracked deferred optimization:
// TODO(username): Replace busy-wait polling with a condition variable.
while (!*map) {
  sched_yield();
}
```
