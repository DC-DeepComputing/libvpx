---
name: libvpx/vp9/round-floating-point-framerate-in-rate-control
description: When computing integer rate control parameters or boosts from floating-point framerate expressions, round with round() before casting instead of truncating with (int)
rubric: correctness
severity: should_fix
---

**Rule.** When computing integer rate control parameters, target sizes, frame
intervals, or keyframe boosts from floating-point framerate expressions (or
bandwidth divided by framerate), round the floating-point result using round()
before casting to an integer. Do not directly truncate floating-point framerate
calculations using raw integer casts.

**Why.** Framerates and rate control factors are floating-point values that
often represent fractional frame rates (such as 23.976 or 29.97 fps) or suffer
from floating-point imprecision. Converting them directly with an integer cast
truncates toward zero rather than rounding to the nearest integer, causing
off-by-one errors in rate control interval targets and keyframe boost
calculations.

## Examples

Bad:

```cpp
kf_boost = VPXMAX(kf_boost, (int)(2 * framerate - 16));
if (rc->frames_since_key < framerate / 2) {
  kf_boost = (int)(kf_boost * rc->frames_since_key / (framerate / 2));
}
```

Good:

```cpp
kf_boost = VPXMAX(kf_boost, (int)round(2 * framerate - 16));
if (rc->frames_since_key < framerate / 2) {
  kf_boost = (int)round(kf_boost * rc->frames_since_key / (framerate / 2));
}
```
