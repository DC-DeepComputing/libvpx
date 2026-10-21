---
name: libvpx/vpx_dsp/specialize-simd-format-helpers-for-inlining
rubric: safety
severity: should_fix
---

**Rule.** In SIMD DSP routines, define separate specialized `static INLINE`
helper functions for each block format or dimension rather than bundling
multiple formats into a single switch dispatcher function.

**Why.** A consolidated dispatcher helper with a switch statement increases
function size and branch complexity, exceeding compiler inlining budgets at -O2
and causing function call overhead in performance-critical pixel loops.
Specialized helpers allow compilers to inline vector instructions directly into
loop bodies.

## Examples

Bad:

```cpp
static INLINE void pack_pixels_with_format(const uint16_t *src,
                                           PixelPackFormat fmt,
                                           ptrdiff_t stride, __m256i *x) {
  switch (fmt) {
    case PACK_8x1: {
      pack_8_pixels(src, x);
      break;
    }
    case PACK_8x2: {
      pack_16_pixels(src, stride, x);
      break;
    }
    default: {
      assert(0);
    }
  }
}
```

Good:

```cpp
static INLINE void pack_8x1_pixels(const uint16_t *src, __m256i *x) {
  pack_8_pixels(src, x);
}

static INLINE void pack_8x2_pixels(const uint16_t *src, ptrdiff_t stride,
                                   __m256i *x) {
  pack_16_pixels(src, stride, x);
}
```
