---
name: libvpx/use-signed-integers-for-pixel-difference-sums
description: When accumulating pixel differences in variance or distortion calculations, use signed integer types like int64_t or int rather than unsigned accumulators
rubric: correctness
severity: should_fix
---

**Rule.** When computing variance, distortion, or block metrics by accumulating
pixel differences (e.g., a[i] - b[i]), use signed integer types (such as
int64_t, int32_t, or int) for the sum accumulator variable and output parameter.
Reserve unsigned integer types (such as uint64_t, uint32_t) for non-negative
metrics like sum of squared errors (sse).

**Why.** Pixel differences can be negative, so accumulating them into an
unsigned integer causes unsigned underflow and wraparound to large positive
values. This leads to incorrect mathematical results during subsequent
averaging, arithmetic right shifts, or conversions.

## Examples

Bad:

```cpp
static void aq_highbd_variance64(const uint8_t *a8, int a_stride,
                                 const uint8_t *b8, int b_stride, int w,
                                 int h, uint64_t *sse, uint64_t *sum) {
  *sum = 0;
  *sse = 0;
  for (int i = 0; i < h; ++i) {
    for (int j = 0; j < w; ++j) {
      const int diff = a[j] - b[j];
      *sum += diff;
      *sse += diff * diff;
    }
  }
}
```

Good:

```cpp
static void aq_highbd_variance64(const uint8_t *a8, int a_stride,
                                 const uint8_t *b8, int b_stride, int w,
                                 int h, uint64_t *sse, int64_t *sum) {
  *sum = 0;
  *sse = 0;
  for (int i = 0; i < h; ++i) {
    for (int j = 0; j < w; ++j) {
      const int diff = a[j] - b[j];
      *sum += diff;
      *sse += diff * diff;
    }
  }
}
```
