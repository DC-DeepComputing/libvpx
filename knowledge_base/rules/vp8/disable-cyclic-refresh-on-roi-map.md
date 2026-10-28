---
name: libvpx/vp8/disable-cyclic-refresh-on-roi-map
description: When configuring explicit ROI maps from external applications, disable conflicting internal segmentation features like cyclic refresh rather than returning an error
rubric: correctness
severity: should_fix
---

**Rule.** When an application configures an explicit region-of-interest (ROI) or
segmentation map via encoder control functions (such as vp8_set_roimap or
vp9_set_roi_map), disable conflicting internal segmentation features (such as
cyclic refresh) rather than returning an error. Ensure internal cyclic refresh
flags (e.g., cyclic_refresh_mode_enabled or cyclic_refresh AQ) are disabled so
external ROI parameters take precedence without configuration failure.

**Why.** Macroblock segmentation in VP8 and VP9 is a shared codec resource used
by both internal cyclic refresh and external ROI mapping. Rejecting ROI
configuration calls when cyclic refresh is active breaks application control,
whereas prioritizing explicit user-provided ROI maps and disabling internal
cyclic refresh honors caller intent.

## Examples

Bad:

```cpp
int vp8_set_roimap(VP8_COMP *cpi, unsigned char *map, ...) {
  // Rejecting ROI configuration when cyclic refresh is enabled:
  if (cpi->cyclic_refresh_mode_enabled) return -1;
  cpi->use_roi_static_threshold = 1;
  return 0;
}
```

Good:

```cpp
int vp8_set_roimap(VP8_COMP *cpi, unsigned char *map, ...) {
  cpi->use_roi_static_threshold = 1;
  // Prioritize explicit application ROI map by disabling internal cyclic refresh:
  cpi->cyclic_refresh_mode_enabled = 0;
  return 0;
}
```
