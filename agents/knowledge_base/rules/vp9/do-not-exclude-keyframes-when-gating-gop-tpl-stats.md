---
name: libvpx/vp9/do-not-exclude-keyframes-when-gating-gop-tpl-stats
description: When gating GOP-level operations such as TPL stats estimation, do not filter out keyframes with frame_type != KEY_FRAME; check GOP update types instead
rubric: correctness
severity: should_fix
---

**Rule.** When gating GOP-level operations (such as TPL stats estimation in
external rate control), do not check frame_type != KEY_FRAME to filter out GOP
start frames. Instead, identify GOP boundaries and distinguish ARF from non-ARF
GOPs using update types (e.g., update_type[1] != ARF_UPDATE) and GOP counter
checks (frames_till_gf_update_due == baseline_gf_interval) so small keyframe
GOPs without ARFs are not skipped.

**Why.** Small keyframe GOPs without an alternate reference frame (ARF) still
require GOP-level operations (such as TPL stats estimation) at the beginning of
the GOP. Excluding keyframes directly via frame_type != KEY_FRAME incorrectly
bypasses GOP-level processing for these GOPs.

## Examples

Bad:

```cpp
static INLINE int ext_rc_tpl_pass(VP9_COMP *cpi) {
  VP9_COMMON *const cm = &cpi->common;
  RATE_CONTROL *const rc = &cpi->rc;
  return cpi->ext_ratectrl.ready &&
         cpi->ext_ratectrl.funcs.send_tpl_gop_stats != NULL &&
         rc->frames_till_gf_update_due == rc->baseline_gf_interval &&
         cm->frame_type != KEY_FRAME &&
         cpi->twopass.gf_group.update_type[1] != ARF_UPDATE;
}
```

Good:

```cpp
static INLINE int should_run_tpl(VP9_COMP *cpi, int gf_group_index) {
  RATE_CONTROL *const rc = &cpi->rc;
  if (!cpi->sf.enable_tpl_model) return 0;
  if (gf_group_index == 1 &&
      cpi->twopass.gf_group.update_type[gf_group_index] == ARF_UPDATE)
    return 1;
  if (cpi->ext_ratectrl.ready &&
      cpi->ext_ratectrl.funcs.send_tpl_gop_stats != NULL &&
      rc->frames_till_gf_update_due == rc->baseline_gf_interval &&
      cpi->twopass.gf_group.update_type[1] != ARF_UPDATE) {
    return 1;
  }
  return 0;
}
```
