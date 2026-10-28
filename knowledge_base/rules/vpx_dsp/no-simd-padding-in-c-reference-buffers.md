---
name: libvpx/vpx_dsp/no-simd-padding-in-c-reference-buffers
description: When declaring scratch buffers in scalar C reference implementations (*_c), do not add SIMD over-read padding unless the buffer is accessed by vector instructions
rubric: correctness
severity: should_fix
---

**Rule.** Do not add SIMD over-read padding or safety margins (such as `+ 16`
bytes) to internal stack scratch buffers or array dimensions in pure C reference
implementations (`*_c`). Only add buffer padding or stride alignment where the
buffer is directly accessed by architecture-specific SIMD routines (e.g., SSE,
NEON, VSX, MSA) that execute vector load instructions beyond scalar boundaries,
or when passing the buffer to RTCD-dispatched functions.

**Why.** Scalar C reference implementations process elements within exact loop
bounds and do not perform vector over-reads. Adding padding to scalar reference
buffers does not resolve SIMD over-read issues on vector backends and
unnecessarily increases stack frame sizes and memory footprint.

## Examples

Bad:

```cpp
#define SUBPIX_VAR(W, H) \
  uint32_t vpx_sub_pixel_variance##W##x##H##_c(...) { \
    uint16_t fdata3[(H + 1) * W]; \
    /* Anti-pattern: +16 padding added to pure C reference buffer */ \
    uint8_t temp2[H * W + 16]; \
    var_filter_block2d_bil_first_pass(src_ptr, fdata3, ...); \
    var_filter_block2d_bil_second_pass(fdata3, temp2, ...); \
    return vpx_variance_c(temp2, W, ...); \
  }
```

Good:

```cpp
#define SUBPIX_VAR(W, H) \
  uint32_t vpx_sub_pixel_variance##W##x##H##_c(...) { \
    uint16_t fdata3[(H + 1) * W]; \
    /* Pure C reference buffer sized to exact dimensions */ \
    uint8_t temp2[H * W]; \
    var_filter_block2d_bil_first_pass(src_ptr, fdata3, ...); \
    var_filter_block2d_bil_second_pass(fdata3, temp2, ...); \
    return vpx_variance_c(temp2, W, ...); \
  }
```
