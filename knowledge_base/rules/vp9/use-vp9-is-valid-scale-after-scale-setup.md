---
name: libvpx/vp9/use-vp9-is-valid-scale-after-scale-setup
description: When checking reference frame validity after initializing scale factors in VP9, use vp9_is_valid_scale rather than redundantly calling valid_ref_frame_size
rubric: readability
severity: should_fix
---

**Rule.** After initializing frame scale factors with
vp9_setup_scale_factors_for_frame(), check reference frame scale validity using
vp9_is_valid_scale(&ref_buf->sf) or vp9_is_scaled(&ref_buf->sf) rather than
re-evaluating dimensions with valid_ref_frame_size(). Re-invoking
valid_ref_frame_size() is redundant because vp9_setup_scale_factors_for_frame()
already evaluates frame dimensions and assigns REF_INVALID_SCALE to invalid
scale factors.

**Why.** vp9_setup_scale_factors_for_frame() internally calls
valid_ref_frame_size() and marks invalid scaling ratios with REF_INVALID_SCALE.
Inspecting vp9_is_valid_scale() reuses this already-computed scaling state and
avoids duplicate dimension validation across encoder and decoder reference setup
routines.

## Examples

Bad:

```cpp
vp9_setup_scale_factors_for_frame(&ref_buf->sf, buf->y_crop_width,
                                  buf->y_crop_height, cm->width,
                                  cm->height);
has_valid_ref_frame |= valid_ref_frame_size(
    buf->y_crop_width, buf->y_crop_height, cm->width, cm->height);
```

Good:

```cpp
vp9_setup_scale_factors_for_frame(&ref_buf->sf, buf->y_crop_width,
                                  buf->y_crop_height, cm->width,
                                  cm->height);
has_valid_ref_frame |= vp9_is_valid_scale(&ref_buf->sf);
```
