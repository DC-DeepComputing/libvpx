---
name: libvpx/vp9/avoid-reference-reordering-in-tpl-estimation
description: When iterating over reference frames in TPL or rate control estimation, use direct loop iteration instead of conditional reference index remapping arrays
rubric: design
severity: should_fix
---

**Rule.** When iterating over reference frames in VP9 TPL model and rate control
estimation, iterate directly over reference frame indices (0 to
MAX_INTER_REF_FRAMES - 1) rather than introducing conditional index remapping
arrays or custom reordering logic. Avoid ad-hoc reference reordering when all
reference frames are evaluated and the statistical impact on rate-distortion
estimation is negligible.

**Why.** Introducing conditional remapping arrays and indirection adds
unnecessary code complexity, branching, and maintenance overhead without
providing meaningful rate-distortion or statistical estimation improvements in
TPL analysis.

## Examples

Bad:

```cpp
int arr[3] = { 0, 1, 2 };
if (cpi->ext_ratectrl.ready &&
    (cpi->ext_ratectrl.funcs.rc_type & VPX_RC_GOP) != 0) {
  if (frame_idx < cpi->twopass.gf_group.gf_group_size) {
    arr[0] = cpi->twopass.gf_group.ext_rc_ref[frame_idx].last_index;
    arr[1] = cpi->twopass.gf_group.ext_rc_ref[frame_idx].golden_index;
    arr[2] = cpi->twopass.gf_group.ext_rc_ref[frame_idx].altref_index;
  }
}
for (int i = 0; i < MAX_INTER_REF_FRAMES; ++i) {
  int rf_idx = arr[i];
  if (rf_idx < 0 || rf_idx >= MAX_INTER_REF_FRAMES) continue;
}
```

Good:

```cpp
for (int rf_idx = 0; rf_idx < MAX_INTER_REF_FRAMES; ++rf_idx) {
  // Directly evaluate reference frame stats
}
```
