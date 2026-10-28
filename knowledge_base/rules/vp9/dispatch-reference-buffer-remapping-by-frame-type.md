---
name: libvpx/vp9/dispatch-reference-buffer-remapping-by-frame-type
description: When iterating over reference frames in loops to remap or disable scaled references, dispatch buffer index updates conditionally based on the active reference frame type
rubric: correctness
severity: should_fix
---

**Rule.** When iterating over reference frames in a loop (e.g., from LAST_FRAME
through ALTREF_FRAME) to remap buffer slots or disable scaled references,
dispatch buffer index updates (such as setting gld_fb_idx or alt_fb_idx to
lst_fb_idx) conditionally based on the matching ref_frame type. Do not assign
frame-specific buffer indices unconditionally across all loop iterations or omit
handling for other non-last references.

**Why.** Unconditional buffer remapping inside a reference frame loop overwrites
buffer indices on unrelated iterations (such as executing golden buffer
remapping during the LAST_FRAME iteration) and fails to remap other reference
types like ALTREF_FRAME, causing corrupted reference selection or
rate-distortion regressions.

## Examples

Bad:

```cpp
for (ref_frame = LAST_FRAME; ref_frame <= ALTREF_FRAME; ++ref_frame) {
  if (vp9_is_scaled(scale_fac)) {
    cpi->ref_frame_flags &= (~flag_list[ref_frame]);
    // BAD: Unconditionally overwrites golden buffer index on every iteration
    if (!svc->use_gf_temporal_ref) {
      svc->gld_fb_idx[sl] = svc->lst_fb_idx[sl];
      cpi->gld_fb_idx = cpi->lst_fb_idx;
    }
  }
}
```

Good:

```cpp
for (ref_frame = LAST_FRAME; ref_frame <= ALTREF_FRAME; ++ref_frame) {
  if (vp9_is_scaled(scale_fac)) {
    cpi->ref_frame_flags &= (~flag_list[ref_frame]);
    // GOOD: Conditionally dispatch buffer remapping by reference frame type
    if (ref_frame == GOLDEN_FRAME) {
      cpi->gld_fb_idx = cpi->lst_fb_idx;
    } else if (ref_frame == ALTREF_FRAME) {
      cpi->alt_fb_idx = cpi->lst_fb_idx;
    }
  }
}
```
