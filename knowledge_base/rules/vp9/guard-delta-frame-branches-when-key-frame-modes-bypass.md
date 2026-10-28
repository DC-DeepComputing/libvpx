---
name: libvpx/vp9/guard-delta-frame-branches-when-key-frame-modes-bypass
description: When branching on frame types where special sub-modes bypass standard key-frame handling, explicitly guard delta-frame branches with cm->frame_type != KEY_FRAME rather than bare else
rubric: correctness
severity: should_fix
---

**Rule.** When branching on frame types where a specialized encoder sub-mode
(such as SVC simulcast mode or custom reference configuration) bypasses the
standard KEY_FRAME branch (e.g., cm->frame_type == KEY_FRAME &&
!svc->simulcast_mode), explicitly guard subsequent delta- or inter-frame
handling blocks with else if (cm->frame_type != KEY_FRAME) rather than using an
unconstrained else. Do not allow key frames in specialized sub-modes to fall
through into delta-frame buffer update or rate control logic.

**Why.** When a key-frame conditional branch excludes a specific sub-mode, key
frames running in that sub-mode evaluate the condition to false. If the
subsequent branch is a bare else, those key frames fall through into delta-frame
logic (such as updating only refresh_last_frame instead of all reference buffer
slots), causing reference buffer corruption and visual artifacts.

## Examples

Bad:

```cpp
if (svc->use_set_ref_frame_config) {
  vp9_svc_update_ref_frame_bypass_mode(cpi);
} else if (cm->frame_type == KEY_FRAME && !svc->simulcast_mode) {
  // Update all reference frame slots on key frame.
  for (i = 0; i < REF_FRAMES; ++i)
    ref_cnt_fb(pool->frame_bufs, &cm->ref_frame_map[i], cm->new_fb_idx);
} else {
  // Bug: Key frames in simulcast mode fall through into delta frame logic!
  if (cpi->refresh_last_frame) {
    ref_cnt_fb(pool->frame_bufs, &cm->ref_frame_map[cpi->lst_fb_idx],
               cm->new_fb_idx);
  }
}
```

Good:

```cpp
if (svc->use_set_ref_frame_config) {
  vp9_svc_update_ref_frame_bypass_mode(cpi);
} else if (cm->frame_type == KEY_FRAME && !svc->simulcast_mode) {
  // Update all reference frame slots on key frame.
  for (i = 0; i < REF_FRAMES; ++i)
    ref_cnt_fb(pool->frame_bufs, &cm->ref_frame_map[i], cm->new_fb_idx);
} else if (cm->frame_type != KEY_FRAME) {
  // Explicitly guarded: only execute for non-key (delta/inter) frames.
  if (cpi->refresh_last_frame) {
    ref_cnt_fb(pool->frame_bufs, &cm->ref_frame_map[cpi->lst_fb_idx],
               cm->new_fb_idx);
  }
}
```
