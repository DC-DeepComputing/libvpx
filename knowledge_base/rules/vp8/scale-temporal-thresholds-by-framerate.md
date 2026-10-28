---
name: libvpx/vp8/scale-temporal-thresholds-by-framerate
description: When defining temporal rate-limiting or frequency constraints on encoder actions like frame dropping, scale frame count thresholds by stream framerate instead of hardcoded constants
rubric: correctness
severity: should_fix
---

**Rule.** When defining temporal rate-limiting, cooldown intervals, or frequency
constraints on encoder actions (such as frame dropping, rate correction, or mode
throttling), scale frame counter thresholds by the stream's frame rate (e.g.,
(int)cpi->framerate or cpi->oxcf.framerate) rather than comparing against
hardcoded frame count constants. Use descriptive names for elapsed frame
counters (such as frames_since_last_drop_overshoot).

**Why.** A hardcoded frame count represents vastly different real-world time
intervals depending on the frame rate (e.g., 8 frames is ~133ms at 60 fps but
800ms at 10 fps). Scaling thresholds by stream framerate ensures consistent
temporal throttling behavior across different frame rates and avoids
over-throttling at low framerates or under-throttling at high framerates.

## Examples

Bad:

```cpp
// Anti-pattern: Hardcoded frame count produces inconsistent throttling across framerates
if (cpi->drop_frames_allowed &&
    cpi->rate_correction_factor < (2.0f * MIN_BPB_FACTOR) &&
    cpi->last_drop_overshoot > 8) {
  // Allow drop frame
}
```

Good:

```cpp
// Scale frame threshold by stream framerate to maintain consistent temporal interval
if (cpi->drop_frames_allowed &&
    cpi->rate_correction_factor < (2.0f * MIN_BPB_FACTOR) &&
    cpi->frames_since_last_drop_overshoot > (int)cpi->framerate) {
  // Allow drop frame
}
```
