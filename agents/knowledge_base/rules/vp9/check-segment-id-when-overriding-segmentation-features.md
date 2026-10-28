---
name: libvpx/vp9/check-segment-id-when-overriding-segmentation-features
description: When overriding segmentation features or block skip decisions (such as resetting seg_skip), explicitly verify that the block's segment_id matches the intended target segment
rubric: correctness
severity: should_fix
---

**Rule.** When overriding or resetting segmentation properties (such as
resetting seg_skip or modifying segment features) based on block content state
or motion analysis, explicitly verify that segment_id matches the intended
target segment (e.g., BACKGROUND_SEG_SKIP_ID or specific cyclic refresh
segments). Do not unconditionally reset or modify segment feature decisions
across all segments without checking segment_id.

**Why.** In VP9, segmentation can be configured concurrently for different
purposes across multiple segment IDs (such as background ROI skipping and cyclic
refresh). Modifying segmentation properties like seg_skip without guarding for
the specific segment_id inadvertently overrides segmentation features on other
active segments, causing visual artifacts or rate-control regressions.

## Examples

Bad:

```cpp
// Anti-pattern: resetting seg_skip based on content state without checking segment_id
if (x->content_state_sb > kLowSadLowSumdiff) {
  seg_skip = 0;
}
```

Good:

```cpp
// Idiomatic: verifying segment_id before overriding segmentation behavior
if (segment_id == BACKGROUND_SEG_SKIP_ID &&
    x->content_state_sb > kLowSadLowSumdiff) {
  seg_skip = 0;
}
```
