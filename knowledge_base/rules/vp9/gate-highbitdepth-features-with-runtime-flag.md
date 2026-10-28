---
name: libvpx/vp9/gate-highbitdepth-features-with-runtime-flag
description: 'When disabling VP9 features unsupported for high-bitdepth, check runtime YV12_FLAG_HIGHBITDEPTH buffer flags rather than using compile-time #if !CONFIG_VP9_HIGHBITDEPTH'
rubric: correctness
severity: should_fix
---

**Rule.** Do not use compile-time #if !CONFIG_VP9_HIGHBITDEPTH to disable
features or speed optimizations that are only unsupported for high-bitdepth
content. Instead, enable the feature generally and gate or disable it at runtime
under #if CONFIG_VP9_HIGHBITDEPTH by checking buffer flags such as
cpi->Source->flags & YV12_FLAG_HIGHBITDEPTH or xd->cur_buf->flags &
YV12_FLAG_HIGHBITDEPTH.

**Why.** In libvpx, high-bitdepth builds (CONFIG_VP9_HIGHBITDEPTH=1) still
encode standard 8-bit (low-bitdepth) source frames. Disabling features with #if
!CONFIG_VP9_HIGHBITDEPTH strips optimizations from 8-bit inputs in high-bitdepth
builds, causing inconsistent encoder behavior and performance across build
configurations.

## Examples

Bad:

```cpp
#if !CONFIG_VP9_HIGHBITDEPTH
  // Enable ML-based partition for low-resolution frames.
  if (!frame_is_intra_only(cm) && cm->width * cm->height <= 352 * 288) {
    sf->nonrd_use_ml_partition = 1;
  }
#endif
```

Good:

```cpp
  // Enable ML-based partition for low-resolution frames.
  if (!frame_is_intra_only(cm) && cm->width * cm->height <= 352 * 288) {
    sf->nonrd_use_ml_partition = 1;
  }
#if CONFIG_VP9_HIGHBITDEPTH
  if (cpi->Source->flags & YV12_FLAG_HIGHBITDEPTH) {
    sf->nonrd_use_ml_partition = 0;
  }
#endif
```
