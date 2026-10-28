---
name: libvpx/use-inline-macro-not-bare-inline-keyword
description: Qualify header, utility, and SIMD helper functions with 'static INLINE' (including 'vpx_config.h') or 'static VPX_FORCE_INLINE' instead of bare 'inline', plain 'static', or bare 'INLINE'
rubric: readability
severity: should_fix
---

**Rule.** Use the project macro 'INLINE' (from 'vpx_config.h') with 'static'
linkage (i.e., 'static INLINE') when defining inline helper functions in C
source (.c) and header (.h) files, and include './vpx_config.h'. In header
files, define short trivial helper and accessor functions directly as 'static
INLINE' rather than implementing them out-of-line in .c files. Never use bare
'inline', plain 'static' for header or hot-path inner-loop utility functions,
bare 'INLINE' without 'static', or compiler-specific keywords ('__inline',
'__forceinline'). Additionally, for performance-critical SIMD vector and DSP
helper functions (such as ARM NEON, x86 SSE/AVX, PowerPC VSX, or MIPS MSA
routines), qualify definitions with 'static VPX_FORCE_INLINE' (from
'vpx_integer.h') rather than plain 'INLINE' to guarantee cross-compiler vector
inlining.

**Why.** libvpx targets C89/C90 across diverse toolchains (including MSVC, GCC,
and Clang) where the bare 'inline' keyword is non-standard and requires
compiler-specific extensions, which the 'INLINE' and 'VPX_FORCE_INLINE' macros
abstract portably.
1. In header files, bare 'INLINE' without 'static' causes external linkage, leading to duplicate symbol definition or missing symbol linker errors across translation units.
2. Plain 'static' without inlining causes unused-function compiler warnings in translation units where the helper is not referenced, and incurs function call overhead or code bloat.
3. Defining trivial accessor and utility helpers directly in headers as 'static INLINE' eliminates function call overhead and enables cross-translation-unit compiler optimizations.
4. In performance-critical SIMD and DSP routines, failing to inline small helper functions incurs call overhead, forces vector registers to spill to the stack, and prevents vector optimizations. 'VPX_FORCE_INLINE' guarantees compiler-enforced inlining across all supported target platforms.

## Examples

Bad:

```cpp
// In header file (.h) or source file (.c):

// Bad 1: Bare 'INLINE' without static linkage causes duplicate symbol linker errors.
INLINE int denoise_svc(const struct VP9_COMP *const cpi) {
  return cpi->oxcf.noise_sensitivity > 0 && cpi->svc.spatial_layer_id == 0;
}

// Bad 2: Plain 'static' in header or hot-path inner loop triggers unused warnings or overhead.
static int mod_index(int sum_dist, int index, int rounding, int strength,
                     int filter_weight) {
  int mod = (sum_dist * 3) / index + rounding;
  return VPXMIN(16, mod >> strength) * filter_weight;
}

// Bad 3: Bare 'inline' keyword or compiler keywords (__inline) are non-portable in C89/MSVC.
static inline int get_lsb(int n) {
  return _tzcnt_u32(n);
}

// Bad 4: Performance-critical SIMD vector helpers should use static VPX_FORCE_INLINE.
static INLINE int16x8_t highbd_quantize_b_neon(
    const tran_low_t *coeff_ptr, tran_low_t *qcoeff_ptr,
    tran_low_t *dqcoeff_ptr, const int16x8_t zbin, const int16x8_t round,
    const int16x8_t quant, const int16x8_t quant_shift,
    const int16x8_t dequant) {
  // Vector quantization logic
}
```

Good:

```cpp
// In header file (.h) or source file (.c):
#include "./vpx_config.h"
#include "vpx_ports/vpx_integer.h"

// Good 1: Qualified with 'static INLINE' for header accessors and utility helpers.
static INLINE int denoise_svc(const struct VP9_COMP *const cpi) {
  return cpi->oxcf.noise_sensitivity > 0 && cpi->svc.spatial_layer_id == 0;
}

// Good 2: Hot-path utility helper marked 'static INLINE'.
static INLINE int mod_index(int sum_dist, int index, int rounding, int strength,
                            int filter_weight) {
  int mod = (sum_dist * 3) / index + rounding;
  return VPXMIN(16, mod >> strength) * filter_weight;
}

// Good 3: Portable 'static INLINE' instead of bare 'inline'.
static INLINE int get_lsb(int n) {
  return _tzcnt_u32(n);
}

// Good 4: Vector SIMD helper qualified with 'static VPX_FORCE_INLINE'.
static VPX_FORCE_INLINE int16x8_t highbd_quantize_b_neon(
    const tran_low_t *coeff_ptr, tran_low_t *qcoeff_ptr,
    tran_low_t *dqcoeff_ptr, const int16x8_t zbin, const int16x8_t round,
    const int16x8_t quant, const int16x8_t quant_shift,
    const int16x8_t dequant) {
  // Vector quantization logic
}
```
