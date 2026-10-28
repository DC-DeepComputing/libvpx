---
name: libvpx/vp9/guard-svc-reference-updates-against-standard-updates
description: When updating VP9 encoder reference frames in SVC mode, conditionally branch between SVC reference updates and standard update_ref_frames to avoid buffer corruption
rubric: correctness
severity: should_fix
---

**Rule.** When executing reference frame updates in the VP9 encoder, do not
invoke the standard update_ref_frames() routine unconditionally before SVC
reference updates on key frames. Gate standard reference updates with an else
branch or condition to prevent update_ref_frames() from overwriting
layer-specific LAST, GOLDEN, and ALTREF reference buffer slots.

**Why.** Standard update_ref_frames() reassigns reference buffer slot indices
(LAST, GOLDEN, ALTREF) on key frames. In SVC mode, reference buffer slots are
managed per spatial and temporal layer; running standard updates without
branching overwrites layer buffer assignments and corrupts reference tracking.

## Examples

Bad:

```cpp
void vp9_update_reference_frames(VP9_COMP *cpi) {
  // Bug: Standard update_ref_frames unconditionally modifies buffer slots.
  update_ref_frames(cpi);
  if (is_one_pass_cbr_svc(cpi) && cpi->common.frame_type == KEY_FRAME) {
    vp9_svc_update_ref_frame_key(cpi);
  }
}
```

Good:

```cpp
void vp9_update_reference_frames(VP9_COMP *cpi) {
  // Correct: Branch between SVC key-frame updates and standard updates.
  if (is_one_pass_cbr_svc(cpi) && cpi->common.frame_type == KEY_FRAME) {
    vp9_svc_update_ref_frame_key(cpi);
  } else {
    update_ref_frames(cpi);
  }
}
```
