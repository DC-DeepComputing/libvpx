---
name: libvpx/vp8/disable-segmentation-on-empty-roi-map
description: When configuring encoder ROI maps, disable segmentation if the map pointer is NULL or all segment deltas and thresholds are zero
rubric: correctness
severity: should_fix
---

**Rule.** When configuring encoder Region-of-Interest (ROI) maps (such as in
vp8_set_roimap or vp9_set_roi_map), verify whether all segment deltas (delta_q,
delta_lf) and thresholds are zero in addition to checking for a NULL map
pointer. If all delta parameters and thresholds are zero or no map is provided,
disable segmentation (disable_segmentation) and return early instead of
activating segmentation for a no-op configuration.

**Why.** When all segment deltas and thresholds are zero, no macroblock segment
modifies quantization, loop filtering, or breakout thresholds. Enabling
segmentation for an all-zero map incurs unnecessary bitstream signaling overhead
and encoder processing without providing any visual or rate-distortion benefit.

## Examples

Bad:

```cpp
int vp8_set_roimap(VP8_COMP *cpi, unsigned char *map, unsigned int rows,
                   unsigned int cols, int delta_q[4], int delta_lf[4],
                   unsigned int threshold[4]) {
  if (!map) {
    disable_segmentation(cpi);
    return 0;
  }
  enable_segmentation(cpi);
  // ...
  return 0;
}
```

Good:

```cpp
int vp8_set_roimap(VP8_COMP *cpi, unsigned char *map, unsigned int rows,
                   unsigned int cols, int delta_q[4], int delta_lf[4],
                   unsigned int threshold[4]) {
  if (!map || (delta_q[0] == 0 && delta_q[1] == 0 && delta_q[2] == 0 &&
               delta_q[3] == 0 && delta_lf[0] == 0 && delta_lf[1] == 0 &&
               delta_lf[2] == 0 && delta_lf[3] == 0 && threshold[0] == 0 &&
               threshold[1] == 0 && threshold[2] == 0 && threshold[3] == 0)) {
    disable_segmentation(cpi);
    return 0;
  }
  enable_segmentation(cpi);
  // ...
  return 0;
}
```
