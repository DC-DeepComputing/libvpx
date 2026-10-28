---
name: libvpx/vp9/account-for-high-bit-depth-in-buffer-allocation
description: When calculating frame, tile, or bitstream buffer allocation sizes from image dimensions, account for high bit depths using image_bps or bit-depth scaling factors
rubric: correctness
severity: should_fix
---

**Rule.** When calculating frame, tile, or bitstream buffer allocation sizes
from image dimensions (such as width and height), account for high bit depths
and pixel formats rather than assuming 8-bit (1 byte per sample) data. Scale
buffer sizing calculations by bits per pixel or sample size (e.g., using
image_bps, (1 + use_highbitdepth), or bit depth factors) to ensure sufficient
capacity for 10-bit and 12-bit streams.

**Why.** High bit depth streams (10-bit and 12-bit) require larger sample
representations (typically 2 bytes per sample or higher bits per pixel) across
image planes. Sizing buffer allocations based solely on pixel dimensions without
scaling for bit depth leads to buffer under-allocation and heap memory
corruption.

## Examples

Bad:

```cpp
static size_t encode_tiles_buffer_alloc_size(const VP9_COMP *cpi) {
  // Assumes 8-bit depth (1 byte per sample), under-allocating for 10/12-bit.
  return (size_t)cpi->oxcf.width * cpi->oxcf.height;
}
```

Good:

```cpp
static size_t encode_tiles_buffer_alloc_size(const VP9_COMP *cpi) {
  const int image_bps = VPXMAX(8, cpi->image_bps);
  return (size_t)cpi->oxcf.width * cpi->oxcf.height * image_bps / 8;
}
```
