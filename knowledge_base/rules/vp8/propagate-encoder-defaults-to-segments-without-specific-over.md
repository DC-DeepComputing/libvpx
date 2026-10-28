---
name: libvpx/vp8/propagate-encoder-defaults-to-segments-without-specific-over
description: When propagating global encoder configuration defaults to per-segment parameters, do not bypass default assignment using broad feature-enabled flags unless parameter-specific overrides are active
rubric: correctness
severity: should_fix
---

**Rule.** When updating or propagating global encoder configuration defaults to
per-segment parameters (such as segment_encode_breakout), do not skip setting
defaults based solely on a coarse feature-enabled flag (such as use_roi). Only
bypass default propagation when specific per-segment override values (such as
use_roi_static_threshold) are explicitly configured and active.

**Why.** High-level features like Region of Interest (ROI) or segmentation can
be enabled to adjust only a subset of segment parameters (such as delta QP or
loopfilter levels) while leaving other parameters unspecified. Guarding default
propagation with a coarse feature-enabled flag prevents unspecified segment
parameters from receiving updated global encoder settings, leaving them with
stale or uninitialized values.

## Examples

Bad:

```cpp
if (!cpi->oxcf.use_roi) {
  int i;
  for (i = 0; i < MAX_MB_SEGMENTS; ++i) {
    cpi->segment_encode_breakout[i] = cpi->oxcf.encode_breakout;
  }
}
```

Good:

```cpp
if (!cpi->use_roi_static_threshold) {
  int i;
  for (i = 0; i < MAX_MB_SEGMENTS; ++i) {
    cpi->segment_encode_breakout[i] = cpi->oxcf.encode_breakout;
  }
}
```
