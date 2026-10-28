---
name: libvpx/vp9/standard-reference-frame-terminology
description: When naming variables or writing comments for encoder reference frame flags (cpi->ref_frame_flags), describe reference availability (last_is_reference) rather than mode selection (picked or selected)
rubric: readability
severity: should_fix
---

**Rule.** When declaring variables, naming boolean flags, or writing comments
documenting checks against encoder reference frame availability flags
(cpi->ref_frame_flags, VP9_LAST_FLAG, VP9_GOLD_FLAG, VP8_LAST_FRAME), describe
reference frame status and availability (e.g., last_is_reference or 'if
LAST_FRAME is not a reference'). Do not describe reference availability as a
frame being 'selected' or 'picked' (such as last_ref_picked).

**Why.** cpi->ref_frame_flags specifies whether a reference frame is active and
available in the current frame or spatial layer. Using mode selection
terminology like 'picked' or 'selected' confuses frame-level reference
availability with block-level reference frame search and mode selection
decisions.

## Examples

Bad:

```cpp
// Variable naming:
int last_ref_picked = cpi->ref_frame_flags & VP9_LAST_FLAG;
if (last_ref_picked && denoiser->denoising_level >= kDenLow) {
  vp9_denoiser_denoise(denoiser, ...);
}

// Documentation / comments:
// Copy block if last reference is not selected.
if ((cpi->ref_frame_flags & VP9_LAST_FLAG) && ...) {
```

Good:

```cpp
// Variable naming:
int last_is_reference = cpi->ref_frame_flags & VP9_LAST_FLAG;
if (last_is_reference && denoiser->denoising_level >= kDenLow) {
  vp9_denoiser_denoise(denoiser, ...);
}

// Documentation / comments:
// Copy block if LAST_FRAME is not a reference.
if ((cpi->ref_frame_flags & VP9_LAST_FLAG) && ...) {
```
