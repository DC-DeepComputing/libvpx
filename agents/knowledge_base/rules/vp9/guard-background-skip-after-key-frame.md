---
name: libvpx/vp9/guard-background-skip-after-key-frame
description: When applying background segmentation or ROI skip optimizations in the VP9 encoder, guard skip decisions with cpi->rc.frames_since_key > FRAMES_NO_SKIPPING_AFTER_KEY
rubric: correctness
severity: should_fix
---

**Rule.** When implementing background segmentation, ROI block skipping, or
cyclic refresh skip optimizations in the VP9 encoder, guard skip decisions with
cpi->rc.frames_since_key > FRAMES_NO_SKIPPING_AFTER_KEY. Do not allow premature
background block skipping on frames immediately following a key frame.

**Why.** Frames immediately following a key frame require full encoding to
establish high-quality reference backgrounds and allow reconstructed quality to
settle. Skipping background blocks too soon after a key frame causes persistent
visual artifacts and quality degradation.

## Examples

Bad:

```cpp
// Anti-pattern: skipping background blocks immediately after key frames
if (cpi->roi.enabled && cpi->roi.skip[BACKGROUND_SEG_SKIP_ID]) {
  if (segment_id == BACKGROUND_SEG_SKIP_ID &&
      x->content_state_sb <= kLowSadLowSumdiff) {
    seg_skip = 1;
  }
}
```

Good:

```cpp
// Idiomatic: guarding background skip until quality stabilizes after key frame
if (cpi->roi.enabled && cpi->roi.skip[BACKGROUND_SEG_SKIP_ID] &&
    cpi->rc.frames_since_key > FRAMES_NO_SKIPPING_AFTER_KEY) {
  if (segment_id == BACKGROUND_SEG_SKIP_ID &&
      x->content_state_sb <= kLowSadLowSumdiff) {
    seg_skip = 1;
  }
}
```
