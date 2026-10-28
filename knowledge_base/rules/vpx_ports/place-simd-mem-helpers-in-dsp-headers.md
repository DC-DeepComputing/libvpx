---
name: libvpx/vpx_ports/place-simd-mem-helpers-in-dsp-headers
description: When defining architecture- or SIMD-specific memory load/store helper functions, place them in subsystem headers like vpx_dsp/<arch>/mem_<simd>.h rather than vpx_ports/mem.h
rubric: design
severity: should_fix
---

**Rule.** Place architecture- or SIMD-specific memory helper functions (such as
unaligned load/store utilities) in the corresponding DSP subsystem header
(`vpx_dsp/<arch>/mem_<simd>.h`, e.g., `vpx_dsp/x86/mem_sse2.h` or
`vpx_dsp/arm/mem_neon.h`) rather than top-level generic porting headers like
`vpx_ports/mem.h`.

**Why.** `vpx_ports/mem.h` is reserved for general, architecture-independent
memory portability macros and definitions across the codebase, whereas
architecture-specific SIMD memory helper routines belong in dedicated DSP
architecture headers where the SIMD routines reside.

## Examples

Bad:

```cpp
// In vpx_ports/mem.h
static INLINE void storeu_uint32(void *dst, uint32_t v) {
  memcpy(dst, &v, sizeof(v));
}
```

Good:

```cpp
// In vpx_dsp/x86/mem_sse2.h (or vpx_dsp/arm/mem_neon.h)
static INLINE void storeu_uint32(void *dst, uint32_t v) {
  memcpy(dst, &v, sizeof(v));
}
```
