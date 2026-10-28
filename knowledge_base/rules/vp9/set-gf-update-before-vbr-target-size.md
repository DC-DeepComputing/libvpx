---
name: libvpx/vp9/set-gf-update-before-vbr-target-size
description: When computing frame target sizes in VP9 one-pass VBR rate control, update key-frame state and call vp9_set_gf_update_one_pass_vbr before target calculation
rubric: correctness
severity: should_fix
---

**Rule.** In VP9 one-pass VBR rate control, update key-frame rate control state
parameters (this_key_frame_forced and frames_to_key) when frame_type ==
KEY_FRAME, and call vp9_set_gf_update_one_pass_vbr() to refresh golden frame
parameters before calling vp9_calc_iframe_target_size_one_pass_vbr() or
vp9_calc_pframe_target_size_one_pass_vbr().

**Why.** In one-pass VBR, frame target size calculations depend on updated
golden frame state (baseline_gf_interval, af_ratio_onepass_vbr). Furthermore,
key frames require tracking whether the frame was forced (current_video_frame !=
0) and resetting frames_to_key to key_freq so subsequent interval budgets and
target sizes remain accurate.

## Examples

Bad:

```cpp
} else if (cpi_->oxcf.rc_mode == VPX_VBR) {
  // Anti-pattern: Missing key frame state updates and gf updates before calculating target size
  if (frame_is_intra_only(cm))
    target = vp9_calc_iframe_target_size_one_pass_vbr(cpi_);
  else
    target = vp9_calc_pframe_target_size_one_pass_vbr(cpi_);
}
```

Good:

```cpp
} else if (cpi_->oxcf.rc_mode == VPX_VBR) {
  if (cm->frame_type == KEY_FRAME) {
    cpi_->rc.this_key_frame_forced = cm->current_video_frame != 0;
    cpi_->rc.frames_to_key = cpi_->oxcf.key_freq;
  }
  vp9_set_gf_update_one_pass_vbr(cpi_);
  if (frame_is_intra_only(cm))
    target = vp9_calc_iframe_target_size_one_pass_vbr(cpi_);
  else
    target = vp9_calc_pframe_target_size_one_pass_vbr(cpi_);
}
```
