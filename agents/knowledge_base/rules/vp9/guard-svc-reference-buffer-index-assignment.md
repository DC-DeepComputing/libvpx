---
name: libvpx/vp9/guard-svc-reference-buffer-index-assignment
description: When configuring VP9 SVC flexible or bypass mode reference buffers, only assign buffer slot indices when their corresponding reference flag is active
rubric: correctness
severity: should_fix
---

**Rule.** In VP9 SVC flexible or bypass mode configuration, only assign active
reference buffer indices (cpi->lst_fb_idx, cpi->gld_fb_idx, cpi->alt_fb_idx)
when their corresponding reference flag (reference_last, reference_golden,
reference_altref) is set. Do not unconditionally copy buffer index values from
vpx_svc_ref_frame_config_t or layer context when the reference frame is
inactive.

**Why.** Callers configuring external SVC reference frames may leave unused
reference buffer indices unassigned (defaulting to 0). Unconditionally assigning
unreferenced buffer indices causes the uncompressed frame header to reference
frame buffer 0 (often the keyframe slot) instead of leaving the reference
disabled or falling back to valid active reference slots.

## Examples

Bad:

```cpp
// In SVC bypass / flexible mode reference setup:
cpi->lst_fb_idx = svc->lst_fb_idx[sl];
cpi->gld_fb_idx = svc->gld_fb_idx[sl];
cpi->alt_fb_idx = svc->alt_fb_idx[sl];
```

Good:

```cpp
// Only assign buffer indices when the corresponding reference is active:
if (svc->reference_last[sl]) cpi->lst_fb_idx = svc->lst_fb_idx[sl];
if (svc->reference_golden[sl]) cpi->gld_fb_idx = svc->gld_fb_idx[sl];
if (svc->reference_altref[sl]) cpi->alt_fb_idx = svc->alt_fb_idx[sl];
```
