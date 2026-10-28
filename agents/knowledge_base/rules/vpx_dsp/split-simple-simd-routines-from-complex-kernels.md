---
name: libvpx/vpx_dsp/split-simple-simd-routines-from-complex-kernels
description: When introducing SIMD or DSP optimizations, split simple routines like copy and average into separate commits before complex multi-tap kernels.
rubric: change_scope
severity: should_fix
---

**Rule.** When adding SIMD or DSP optimizations, split large changes into
smaller, focused commits. Separate simple, low-risk routines (such as copy,
average, or basic data movement) from complex algorithmic kernels (such as
multi-tap convolution, subpel interpolation, or large transforms) into distinct
CLs.

**Why.** Simpler routines like copy and average are low-risk and fast to review,
while complex multi-tap filter and transform kernels require deeper analysis.
Splitting them keeps CL sizes manageable, speeds up code review, reduces rebase
conflicts, and allows straightforward optimizations to land independently
without being blocked by complex reviews.

## Examples

Bad:

```cpp
// Bundling copy, avg, and complex 8-tap convolution kernels in one large CL:
void vpx_highbd_convolve_copy_avx2(const uint16_t *src, ptrdiff_t src_stride,
                                   uint16_t *dst, ptrdiff_t dst_stride, int w,
                                   int h);
void vpx_highbd_convolve_avg_avx2(const uint16_t *src, ptrdiff_t src_stride,
                                  uint16_t *dst, ptrdiff_t dst_stride, int w,
                                  int h);
void vpx_highbd_convolve8_avx2(const uint16_t *src, ptrdiff_t src_stride,
                               uint16_t *dst, ptrdiff_t dst_stride,
                               const InterpKernel *filter, int x0_q4,
                               int x_step_q4, int y0_q4, int y_step_q4, int w,
                               int h);
```

Good:

```cpp
// Commit 1 (simple, low-risk routines):
void vpx_highbd_convolve_copy_avx2(const uint16_t *src, ptrdiff_t src_stride,
                                   uint16_t *dst, ptrdiff_t dst_stride, int w,
                                   int h);
void vpx_highbd_convolve_avg_avx2(const uint16_t *src, ptrdiff_t src_stride,
                                  uint16_t *dst, ptrdiff_t dst_stride, int w,
                                  int h);

// Commit 2 (complex filtering kernels submitted separately):
void vpx_highbd_convolve8_avx2(const uint16_t *src, ptrdiff_t src_stride,
                               uint16_t *dst, ptrdiff_t dst_stride,
                               const InterpKernel *filter, int x0_q4,
                               int x_step_q4, int y0_q4, int y_step_q4, int w,
                               int h);
```
