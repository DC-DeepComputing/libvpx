---
name: libvpx/vp9/verify-nonzero-reference-dimensions-before-scaling-check
description: When validating frame dimensions against prior reference frames using valid_ref_frame_size, verify that reference dimensions are non-zero before evaluating scaling constraints
rubric: correctness
severity: should_fix
---

**Rule.** When validating current frame dimensions against previously coded
reference frame sizes (such as with valid_ref_frame_size()), verify that the
reference dimensions (e.g., last_coded_width and last_coded_height) are positive
and non-zero before checking scaling limits. Do not invoke relative scaling
validation when reference dimensions are uninitialized or zero.

**Why.** Prior reference dimensions like last_coded_width and last_coded_height
are initialized to zero and remain zero until the first frame is encoded.
Because valid_ref_frame_size(ref_w, ref_h, w, h) checks w <= 16 * ref_w, zero
reference dimensions cause the check to evaluate to false and falsely trigger
invalid-reference or forced-keyframe logic when encoder settings are updated
prior to encoding the first frame.

## Examples

Bad:

```cpp
if (!valid_ref_frame_size(ctx->cpi->last_coded_width,
                          ctx->cpi->last_coded_height, cfg->g_w,
                          cfg->g_h) ||
    ...) {
  force_key = 1;
}
```

Good:

```cpp
if ((ctx->cpi->last_coded_width && ctx->cpi->last_coded_height &&
     !valid_ref_frame_size(ctx->cpi->last_coded_width,
                           ctx->cpi->last_coded_height, cfg->g_w,
                           cfg->g_h)) ||
    ...) {
  force_key = 1;
}
```
