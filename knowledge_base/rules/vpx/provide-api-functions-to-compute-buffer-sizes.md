---
name: libvpx/vpx/provide-api-functions-to-compute-buffer-sizes
description: When designing public APIs that accept caller-allocated buffers with complex layout or alignment, provide a dedicated sizing function rather than documented calculation formulas
rubric: correctness
severity: should_fix
---

**Rule.** Provide a dedicated public API function (such as vpx_img_buf_size) to
compute the required buffer size when an API function (such as vpx_img_wrap)
accepts caller-allocated buffers with non-trivial layout, stride alignment,
planar formats, or bit-depth calculations. Do not rely solely on doc comments
containing multi-step calculation formulas for callers to implement manually.

**Why.** Requiring external callers to replicate complex internal layout,
stride, and chroma-subsampling arithmetic from documentation is error-prone and
frequently causes buffer under-allocation and out-of-bounds memory corruption.
Encapsulating the calculation in a public API function ensures callers allocate
the exact required capacity and insulates applications from internal layout
changes.

## Examples

Bad:

```cpp
/*!\\brief Wrap caller-allocated memory into an image structure.
 * \\note \\a img_data must have at least the following allocated size:
 * \\code
 * align = (1 << x_chroma_shift) - 1;
 * w = (d_w + align) & ~align;
 * alloc_size = (fmt & VPX_IMG_FMT_PLANAR) ? (uint64_t)h * s * bps / 8 : (uint64_t)h * s;
 * \\endcode
 */
vpx_image_t *vpx_img_wrap(vpx_image_t *img, vpx_img_fmt_t fmt,
                          unsigned int d_w, unsigned int d_h,
                          unsigned int stride_align,
                          unsigned char *img_data);
```

Good:

```cpp
/*!\\brief Calculates the minimum buffer size required for an image.
 * \\return Required buffer size in bytes, or 0 on invalid parameters.
 */
size_t vpx_img_buf_size(vpx_img_fmt_t fmt, unsigned int d_w,
                        unsigned int d_h, unsigned int stride_align);

/*!\\brief Wrap caller-allocated memory into an image structure.
 * \\note \\a img_data size must be at least vpx_img_buf_size().
 */
vpx_image_t *vpx_img_wrap(vpx_image_t *img, vpx_img_fmt_t fmt,
                          unsigned int d_w, unsigned int d_h,
                          unsigned int stride_align,
                          unsigned char *img_data);
```
