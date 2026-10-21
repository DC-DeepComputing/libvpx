---
name: libvpx/vpx_dsp/reuse-intermediate-stack-buffers
rubric: safety
severity: should_fix
---

**Rule.** When implementing multi-stage DSP operations (such as multi-pass 2D
filtering, convolution, compound averaging, or variance calculations), reuse
existing intermediate stack scratch buffers across sequential pipeline stages
once their previous data is no longer needed. Avoid declaring extra temporary
stack arrays or DECLARE_ALIGNED buffers when an already-allocated buffer has
sufficient capacity and an expired lifetime.

**Why.** Declaring redundant stack buffers increases stack frame footprint and
worsens data cache locality. Reusing scratch buffers minimizes stack memory
pressure and keeps intermediate data in hot cache lines.

## Examples

Bad:

```cpp
DECLARE_ALIGNED(16, uint8_t, fdata3[n * (m + 1)]);
DECLARE_ALIGNED(16, uint8_t, temp2[n * m]);
DECLARE_ALIGNED(16, uint8_t, temp3[n * m]);

var_filter_block2d_bil_w8(a, fdata3, a_stride, 1, m + 1, filter_x);
var_filter_block2d_bil_w8(fdata3, temp2, n, n, m, filter_y);

// Allocating a separate temp3 buffer when fdata3 is no longer needed
vpx_comp_avg_pred(temp3, second_pred, n, m, temp2, n);
return vpx_variance8x8(temp3, n, b, b_stride, sse);
```

Good:

```cpp
DECLARE_ALIGNED(16, uint8_t, fdata3[n * (m + 1)]);
DECLARE_ALIGNED(16, uint8_t, temp2[n * m]);

var_filter_block2d_bil_w8(a, fdata3, a_stride, 1, m + 1, filter_x);
var_filter_block2d_bil_w8(fdata3, temp2, n, n, m, filter_y);

// Reuse fdata3 buffer for average prediction since stage 1 data is dead
vpx_comp_avg_pred(fdata3, second_pred, n, m, temp2, n);
return vpx_variance8x8(fdata3, n, b, b_stride, sse);
```
