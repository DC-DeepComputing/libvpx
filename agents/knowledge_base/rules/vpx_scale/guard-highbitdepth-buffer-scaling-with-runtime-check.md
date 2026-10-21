---
name: libvpx/vpx_scale/guard-highbitdepth-buffer-scaling-with-runtime-check
rubric: safety
severity: should_fix
---

**Rule.** Do not unconditionally increase frame buffer borders, strides, row
alignments (row_alignment), or allocation alignments (alloc_alignment) globally
for all targets. Guard 16-bit pixel layout scaling under CONFIG_VP9_HIGHBITDEPTH
with runtime use_highbitdepth / YV12_FLAG_HIGHBITDEPTH checks, and guard
architecture-specific SIMD alignment or border increases (such as 64-byte
alignment or enlarged VP9_DEC_BORDER_IN_PIXELS for x86 AVX-512) behind target
architecture macros (e.g., #if VPX_ARCH_X86 || VPX_ARCH_X86_64), retaining
baseline values (e.g., 32-byte alignment) for non-x86 or portable targets.

**Why.** Increasing frame buffer borders, strides, and alignments enlarges
memory allocations and cache pressure for every frame. Unconditional global
increases penalize 8-bit streams and non-x86 targets (such as ARM NEON or
baseline architectures) with unnecessary memory overhead and layout constraints
without providing performance benefits.

## Examples

Bad:

```cpp
// Unconditional global border and alignment increases for all platforms
#define VP9_DEC_BORDER_IN_PIXELS 64
int row_alignment = 64;
const int alloc_alignment = 64;

#if CONFIG_VP9_HIGHBITDEPTH
// Unconditional compile-time scaling
int border = border_in_pixel << 1;
row_alignment <<= 1;
#else
int border = border_in_pixel;
#endif
```

Good:

```cpp
// Guard increased SIMD borders and alignments behind target architecture macros
#if VPX_ARCH_X86 || VPX_ARCH_X86_64
int row_alignment = 64;
const int alloc_alignment = 64;
#else
#define VP9_DEC_BORDER_IN_PIXELS 32
int row_alignment = 32;
const int alloc_alignment = 32;
#endif

// Guard high bit-depth scaling with runtime check
int border = border_in_pixel;
#if CONFIG_VP9_HIGHBITDEPTH
if (use_highbitdepth) {
  border <<= 1;
  row_alignment <<= 1;
}
#endif
```
