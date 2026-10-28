---
name: libvpx/remove-duplicate-local-extern-declarations
description: Include declaring or RTCD headers in implementation files to verify prototypes rather than writing ad-hoc extern/forward declarations or omitting headers, and mark TU-local functions static
rubric: readability
severity: should_fix
---

**Rule.** In C and C++ source files (.c, .cc, .cpp), do not write manual forward
function declarations, typedef prototypes, or ad-hoc inline 'extern'
declarations, and do not omit module header includes. For shared functions, C
reference routines, and architecture-specific SIMD/DSP functions dispatched via
runtime CPU detection (RTCD), #include the corresponding declaring header or
generated RTCD header (e.g., #include "./vpx_dsp_rtcd.h", #include
"./vp8_rtcd.h", #include "./vp9_rtcd.h", or #include "./vpx_scale_rtcd.h") as
the single source of truth to validate function definitions against prototypes
at compile time. For translation-unit-local helper functions (including when
resolving -Wmissing-declarations warnings), mark the function definition static
(and INLINE where appropriate) rather than adding ad-hoc forward declarations.

**Why.** Declaring headers and generated RTCD headers establish the single
canonical source of truth for function prototypes. Writing manual 'extern' or
forward declarations directly inside .c files—or omitting RTCD headers in
SIMD/DSP implementation files—bypasses compile-time prototype verification
between callers and definitions, risking silent signature drift, incorrect
parameter types, and ABI divergences between definitions and dispatch tables.
Properly including declaring and RTCD headers ensures compiler validation, while
marking TU-local functions static enforces internal linkage and satisfies
-Wmissing-declarations without leaking symbols.

## Examples

Bad:

```cpp
// 1. Missing RTCD header in SIMD / DSP implementation file:
#include <arm_neon.h>
#include "vpx_dsp/arm/transpose_neon.h"

void vpx_lpf_horizontal_16_neon(unsigned char *s, int pitch,
                                const unsigned char *blimit,
                                const unsigned char *limit,
                                const unsigned char *thresh) {
  // Implementation without RTCD prototype validation
}

// 2. Silencing -Wmissing-declarations with a .c forward declaration or typedef:
filter8_1dfunction vpx_filter_block1d16_h4_ssse3;
void vpx_filter_block1d16_h4_ssse3(const uint8_t *src, ...) { /* ... */ }

// 3. Inline extern declaration across translation units instead of header include:
extern int vp8cx_create_encoder_threads(VP8_COMP *cpi);
```

Good:

```cpp
// 1. Include generated RTCD header to validate prototype:
#include <arm_neon.h>
#include "./vpx_dsp_rtcd.h"
#include "vpx_dsp/arm/transpose_neon.h"

void vpx_lpf_horizontal_16_neon(unsigned char *s, int pitch,
                                const unsigned char *blimit,
                                const unsigned char *limit,
                                const unsigned char *thresh) {
  // Implementation verified against RTCD prototype
}

// 2. Mark translation-unit-local functions static (and INLINE if applicable):
static void vpx_filter_block1d16_h4_ssse3(const uint8_t *src, ...) { /* ... */ }

// 3. Include declaring header for cross-file functions:
#if CONFIG_MULTITHREAD
#include "vp8/encoder/ethreading.h"
#endif
```
