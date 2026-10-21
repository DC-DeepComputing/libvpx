---
name: libvpx/add-rounding-factor-before-division-for-ceiling-arithmetic
rubric: correctness
severity: should_fix
---

**Rule.** When computing ceiling integer division (such as calculating
subsampled chroma plane dimensions for YUV 4:2:0/4:2:2 or block counts), add the
rounding bias before dividing: use `(val + div - 1) / div`, `(dim + 1) / 2`, or
`(dim + chroma_shift) >> chroma_shift`. Do not use truncated integer division
(`dim / 2` or `dim >> 1`), which underestimates sample counts on odd dimensions,
and do not add after division (`dim / 2 + 1`), which overestimates even
dimensions.

**Why.** Subsampled chroma formats (such as I420 or YV12) and ceiling divisions
require `ceil(dim / 2)` samples. Truncated integer division (`dim / 2`) produces
an off-by-one undercount on odd dimensions (e.g., width 41 yields 20 instead of
21), leaving boundary chroma rows or columns uninitialized or misaligned.
Conversely, adding after integer division (`dim / 2 + 1`) overestimates even
dimensions (e.g., width 40 yields 21 instead of 20), risking out-of-bounds
buffer writes, memory corruption, or incorrect layout computations.

## Examples

Bad:

```cpp
// Truncated division drops extra sample on odd dimensions (e.g., 41 -> 20):
FillPlane(img->planes[VPX_PLANE_U], img->stride[VPX_PLANE_U],
          cfg.g_w / 2, cfg.g_h / 2, 0x40);

// Post-division addition overestimates even dimensions (e.g., 40 -> 21):
const int uv_width = width / 2 + 1;
const int uv_height = height / 2 + 1;
FillPlane(img->planes[VPX_PLANE_U], img->stride[VPX_PLANE_U],
          uv_width, uv_height, 0x40);
```

Good:

```cpp
// Add rounding bias before division for exact ceiling calculation:
FillPlane(img->planes[VPX_PLANE_U], img->stride[VPX_PLANE_U],
          (cfg.g_w + 1) / 2, (cfg.g_h + 1) / 2, 0x40);

// Or when using explicit chroma shift variables:
const int uv_w = (cfg.g_w + img->x_chroma_shift) >> img->x_chroma_shift;
const int uv_h = (cfg.g_h + img->y_chroma_shift) >> img->y_chroma_shift;
FillPlane(img->planes[VPX_PLANE_U], img->stride[VPX_PLANE_U],
          uv_w, uv_h, 0x40);
```
