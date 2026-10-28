---
name: libvpx/extract-large-dsp-branches-to-inline-helpers
description: When implementing complex DSP or SIMD routines with large distinct execution branches or fast paths, extract them into static INLINE helper functions
rubric: design
severity: should_fix
---

**Rule.** Extract large, distinct execution branches (such as 1D fast-paths,
single-pass filtering, or specialized block dimensions) in complex DSP and SIMD
routines into dedicated static INLINE helper functions. Keep the main
entry-point function focused on dispatching between fast paths and general
multi-pass logic rather than open-coding dozens of lines of SIMD intrinsics
inside inline if branches.

**Why.** Inlining expansive SIMD code blocks inside conditional branches
obscures top-level control flow, bloats entry-point functions, and complicates
maintenance across sibling routines. Factoring distinct branches into static
INLINE helpers preserves zero-overhead compiler inlining while keeping dispatch
logic clean and readable.

## Examples

Bad:

```cpp
void vp8_sixtap_predict4x4_neon(unsigned char *src_ptr, int src_pixels_per_line,
                                int xoffset, int yoffset,
                                unsigned char *dst_ptr, int dst_pitch) {
  if (xoffset == 0) {  // 1D second-pass filter only
    // 50+ lines of NEON vector loads, filtering, accumulation, and stores
    ...
    return;
  }
  // 100+ lines for two-pass 2D filtering
  ...
}
```

Good:

```cpp
static INLINE void yonly4x4(const unsigned char *src, int src_stride,
                            int filter_offset, unsigned char *dst,
                            int dst_stride) {
  // 50+ lines of NEON vector loads, filtering, accumulation, and stores
  ...
}

void vp8_sixtap_predict4x4_neon(unsigned char *src_ptr, int src_pixels_per_line,
                                int xoffset, int yoffset,
                                unsigned char *dst_ptr, int dst_pitch) {
  if (xoffset == 0) {  // 1D second-pass filter only
    yonly4x4(src_ptr, src_pixels_per_line, yoffset, dst_ptr, dst_pitch);
    return;
  }
  // Two-pass 2D filtering
  ...
}
```
