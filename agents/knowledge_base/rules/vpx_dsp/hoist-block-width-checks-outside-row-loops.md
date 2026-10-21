---
name: libvpx/vpx_dsp/hoist-block-width-checks-outside-row-loops
rubric: safety
severity: should_fix
---

**Rule.** In SIMD 2D block DSP functions, hoist invariant block width and
dimension checks outside the row processing loop into top-level width branches
rather than evaluating width conditions inside inner row loops or row helper
functions. Structure the function with top-level branches for specific block
dimensions (such as widths 64, 32, 16, 8, and 4) where each branch executes an
outer height loop with width-tailored vector operations and row unrolling.

**Why.** Block width is constant across the entire 2D block operation.
Evaluating width conditions inside an inner row loop adds per-row branch
prediction overhead in performance-critical code. Hoisting width checks to the
top level eliminates hot-loop branching and allows tailoring vector widths (e.g.
256-bit vs 128-bit) and row-unrolling factors specifically to each block size.

## Examples

Bad:

```cpp
// Branching on width inside inner row loop or row helper
static INLINE void convolve_copy_row(const uint16_t *src, uint16_t *dst,
                                     int width) {
  if (width >= 32) {
    /* 256-bit load and store */
  } else if (width >= 16) {
    /* 128-bit load and store */
  }
}

do {
  convolve_copy_row(src, dst, width);
  src += src_stride;
  dst += dst_stride;
  h -= 1;
} while (h > 0);
```

Good:

```cpp
// Hoist width checks to top-level branches outside the row loop
if (width > 32) {  // width = 64
  do {
    /* process 64-wide row with 256-bit SIMD */
    src += src_stride;
    dst += dst_stride;
    h -= 1;
  } while (h > 0);
} else if (width > 16) {  // width = 32
  do {
    /* process 32-wide row */
    src += src_stride;
    dst += dst_stride;
    h -= 1;
  } while (h > 0);
} else if (width > 8) {  // width = 16
  do {
    /* unroll 2 rows per iteration for smaller widths */
    src += 2 * src_stride;
    dst += 2 * dst_stride;
    h -= 2;
  } while (h > 0);
}
```
