---
name: libvpx/vp9/preserve-pre-encode-overshoot-detection-when-post-encode-dro
description: In VP9 CBR rate control, do not bypass pre-encode overshoot detection or QP adjustments when post-encode frame dropping is enabled
rubric: correctness
severity: should_fix
---

**Rule.** Do not disable or bypass pre-encode overshoot detection and rate
control adjustments (such as FAST_DETECTION_MAXQ on high source SAD) when
post-encode frame dropping (use_post_encode_drop) is enabled. Keep pre-encode QP
adjustments active so frame sizes are reduced proactively before falling back to
dropping the encoded frame.

**Why.** Post-encode frame dropping is a last-resort fallback when an encoded
frame severely exceeds the buffer budget. Bypassing pre-encode overshoot
heuristics causes high-motion frames to encode at lower QP and needlessly exceed
buffer limits, triggering avoidable post-encode frame drops.

## Examples

Bad:

```cpp
if (!cpi->rc.use_post_encode_drop &&
    cpi->sf.overshoot_detection_cbr_rt == FAST_DETECTION_MAXQ &&
    (cpi->rc.high_source_sad ||
     (cpi->use_svc && cpi->svc.high_source_sad_superframe))) {
  if (vp9_encodedframe_overshoot(cpi, -1, &q)) {
    vp9_set_quantizer(cm, q);
    vp9_set_variance_partition_thresholds(cpi, q, 0);
  }
}
```

Good:

```cpp
if (cpi->sf.overshoot_detection_cbr_rt == FAST_DETECTION_MAXQ &&
    (cpi->rc.high_source_sad ||
     (cpi->use_svc && cpi->svc.high_source_sad_superframe))) {
  if (vp9_encodedframe_overshoot(cpi, -1, &q)) {
    vp9_set_quantizer(cm, q);
    vp9_set_variance_partition_thresholds(cpi, q, 0);
  }
}
```
