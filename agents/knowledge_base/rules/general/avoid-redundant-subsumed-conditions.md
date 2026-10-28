---
name: libvpx/avoid-redundant-subsumed-conditions
description: When structuring nested or sequential conditional statements, omit redundant condition checks that are already logically subsumed by outer or preceding guards
rubric: readability
severity: should_fix
---

**Rule.** Do not include redundant condition checks in nested or sequential
conditionals when outer guards, preceding branches, or stricter state values
already guarantee the invariant. Simplify control flow by eliminating checks for
conditions that cannot logically occur once preceding guards have passed.

**Why.** Redundant checks that are already guaranteed by earlier conditions add
unnecessary cognitive load, clutter control flow, and mislead readers into
thinking those invariants could fail within the guarded scope.

## Examples

Bad:

```cpp
// Outer guard guarantees frames_since_key > 20 (so is_key_frame is false)
if (cpi->roi.enabled && cpi->rc.frames_since_key > FRAMES_NO_SKIPPING) {
  const int is_key_frame = frame_is_intra_only(cm);
  if (segment_id == BACKGROUND_SEG_SKIP_ID &&
      x->content_state_sb > kLowSadThreshold) {
    seg_skip = 0;
  }
  // Redundant !is_key_frame check already guaranteed by frames_since_key > 20
  if (!x->zero_temp_sad_source && !seg_skip && !is_key_frame) {
    apply_roi_skip(cpi, x);
  }
}
```

Good:

```cpp
if (cpi->roi.enabled && cpi->rc.frames_since_key > FRAMES_NO_SKIPPING &&
    x->content_state_sb > kLowSadThreshold) {
  seg_skip = 0;
  if (!x->zero_temp_sad_source && !seg_skip) {
    apply_roi_skip(cpi, x);
  }
}
```
