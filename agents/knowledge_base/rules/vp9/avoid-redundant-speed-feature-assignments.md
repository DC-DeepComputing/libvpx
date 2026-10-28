---
name: libvpx/vp9/avoid-redundant-speed-feature-assignments
description: When configuring cumulative speed features in sequential if (speed >= N) blocks, avoid redundantly reassigning settings already established at lower speeds
rubric: readability
severity: nit
---

**Rule.** In cumulative speed-feature setup routines executing sequential if
(speed >= N) blocks, do not redundantly reassign speed feature fields (sf->...)
to values already assigned under identical conditions at lower speed levels.
Only assign fields that are modified or overridden for the current speed
threshold.

**Why.** Higher-speed configuration blocks inherit all speed feature assignments
from preceding lower-speed blocks. Re-evaluating or reassigning identical values
adds visual clutter, obscures where overrides actually take effect, and
increases maintenance overhead when thresholds change.

## Examples

Bad:

```cpp
if (speed >= 5) {
  if (!frame_is_intra_only(cm) && cm->width >= 360 && cm->height >= 360) {
    sf->partition_search_type = ML_BASED_PARTITION;
  }
}
if (speed >= 6) {
  // Bad: redundantly reassigns ML_BASED_PARTITION identically to speed >= 5
  if (!frame_is_intra_only(cm) && cm->width >= 360 && cm->height >= 360) {
    sf->partition_search_type = ML_BASED_PARTITION;
  } else {
    sf->partition_search_type = VAR_BASED_PARTITION;
  }
}
```

Good:

```cpp
if (speed >= 5) {
  if (!frame_is_intra_only(cm) && cm->width >= 360 && cm->height >= 360) {
    sf->partition_search_type = ML_BASED_PARTITION;
  }
}
if (speed >= 6) {
  // Good: only specifies the override for frames not matching the condition
  if (frame_is_intra_only(cm) || cm->width < 360 || cm->height < 360) {
    sf->partition_search_type = VAR_BASED_PARTITION;
  }
}
```
