---
name: libvpx/vpx_dsp/use-existing-simd-memory-helpers
description: When implementing or porting SIMD DSP routines, use existing codebase memory helper headers and standard intrinsics instead of introducing redundant synonym wrapper headers
rubric: readability
severity: should_fix
---

**Rule.** When implementing or porting SIMD routines (e.g., SSE2, SSSE3, AVX2,
or NEON in vpx_dsp), reuse existing codebase helper utilities (such as
vpx_dsp/x86/mem_sse2.h or vpx_dsp/arm/mem_neon.h) and standard compiler
intrinsics. Do not introduce duplicate synonym wrapper headers, foreign porting
abstraction layers, or redundant alias functions for operations already
supported by existing project helpers.

**Why.** Reusing established codebase helper utilities and standard intrinsics
avoids code duplication, reduces header maintenance overhead, and ensures
consistent memory-access patterns across libvpx DSP modules.

## Examples

Bad:

```cpp
#include "vpx_dsp/x86/synonyms.h"

void process_block(const uint8_t *src, int stride) {
  const __m128i v_a0 = xx_loadl_32(src);
  const __m128i v_a1 = xx_loadl_32(src + stride);
  // ...
}
```

Good:

```cpp
#include "vpx_dsp/x86/mem_sse2.h"

void process_block(const uint8_t *src, int stride) {
  const __m128i v_a0 = load_unaligned_u32(src);
  const __m128i v_a1 = load_unaligned_u32(src + stride);
  // ...
}
```
