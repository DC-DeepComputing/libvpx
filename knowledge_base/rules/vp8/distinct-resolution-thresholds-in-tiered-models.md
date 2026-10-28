---
name: libvpx/vp8/distinct-resolution-thresholds-in-tiered-models
description: When implementing multi-tier resolution models in rate control or filtering, ensure conditional branches test distinct, ordered resolution thresholds rather than duplicating adjacent threshold checks
rubric: correctness
severity: should_fix
---

**Rule.** When implementing multi-tier resolution models or resolution-dependent
parameter tuning (such as rate control, loop filter level selection, or noise
estimation), verify that each branch in an if / else if chain tests a distinct,
strictly ordered resolution threshold (e.g., QVGA 320 * 240 vs. VGA 640 * 480).
Avoid duplicating adjacent threshold checks, which causes lower-tier branches to
shadow subsequent conditions or apply incorrect model coefficients.

**Why.** Duplicating a resolution threshold in consecutive if/else if branches
makes subsequent branches unreachable or causes lower-resolution frames to
incorrectly receive parameters tuned for higher-tier resolutions, degrading
video quality or rate control stability.

## Examples

Bad:

```cpp
if (cm->Width * cm->Height <= 640 * 480) {
  cm->filter_level = static_cast<int>(0.352685 * qp + 2.957774);
} else if (cm->Width * cm->Height <= 640 * 480) {
  cm->filter_level = static_cast<int>(0.485069 * qp - 0.534462);
} else {
  cm->filter_level = static_cast<int>(0.314875 * qp + 7.959003);
}
```

Good:

```cpp
if (cm->Width * cm->Height <= 320 * 240) {
  cm->filter_level = static_cast<int>(0.352685 * qp + 2.957774);
} else if (cm->Width * cm->Height <= 640 * 480) {
  cm->filter_level = static_cast<int>(0.485069 * qp - 0.534462);
} else {
  cm->filter_level = static_cast<int>(0.314875 * qp + 7.959003);
}
```
