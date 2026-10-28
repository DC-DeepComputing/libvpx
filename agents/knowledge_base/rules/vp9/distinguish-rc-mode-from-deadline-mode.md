---
name: libvpx/vp9/distinguish-rc-mode-from-deadline-mode
description: When documenting encoder quality or speed settings in VP9, refer to oxcf.mode as deadline mode and reserve rc_mode for rate control modes
rubric: readability
severity: nit
---

**Rule.** Do not use 'rc_mode' to refer to encoder deadline or quality modes
(GOOD, BEST, REALTIME; oxcf.mode) in comments or documentation. Reserve
'rc_mode' exclusively for rate control modes (enum vpx_rc_mode / oxcf.rc_mode),
and refer to deadline and quality settings as 'deadline mode' or 'quality mode'.

**Why.** In libvpx and VP9, 'rc_mode' specifically denotes the rate control mode
(such as VPX_VBR, VPX_CBR, VPX_CQ, or VPX_Q), whereas 'oxcf.mode' represents the
deadline/quality mode (GOOD, BEST, REALTIME). Conflating the two in comments
creates ambiguity and confuses maintainers about whether rate control or encoder
speed/quality heuristics are being referenced.

## Examples

Bad:

```cpp
// For all speeds for rt mode: if the rc_mode changed (was good/best quality
// on previous frame and now is realtime) set nonrd_keyframe to 1 to avoid
// entering rd pickmode. This causes issues, such as: b/310663186.
if (cpi->oxcf.mode != cpi->deadline_mode_previous_frame)
  sf->nonrd_keyframe = 1;
```

Good:

```cpp
// For all speeds for rt mode: if the deadline mode changed (was good/best
// quality on previous frame and now is realtime) set nonrd_keyframe to 1 to
// avoid entering rd pickmode. This causes issues, such as: b/310663186.
if (cpi->oxcf.mode != cpi->deadline_mode_previous_frame)
  sf->nonrd_keyframe = 1;
```
