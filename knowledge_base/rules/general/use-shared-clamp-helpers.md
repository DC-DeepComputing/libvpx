---
name: libvpx/use-shared-clamp-helpers
description: When clamping or bounding numeric values, use standard vpx_dsp_common.h helpers (VPXMIN, VPXMAX, clamp, clip_pixel) instead of manual branching, ternary expressions, or local CLAMP macros
rubric: readability
severity: should_fix
---

**Rule.** When bounding, clamping, or clipping numeric variables or expressions,
use standard helpers from vpx_dsp/vpx_dsp_common.h—such as VPXMIN(), VPXMAX(),
clamp(), fclamp(), lclamp(), clip_pixel(), or clip_pixel_highbd()—instead of
open-coded if/if-else conditionals, ternary expressions, or custom local
CLAMP/CLIP macros. Include vpx_dsp/vpx_dsp_common.h rather than defining ad-hoc
clamping logic or local helper macros.

**Why.** Using standard bounding helpers from vpx_dsp/vpx_dsp_common.h
eliminates multi-line conditional boilerplate, prevents redundant branching, and
avoids code duplication across modules. Additionally, shared inline helpers
provide type-safe clamping with single argument evaluation, avoiding side-effect
risks inherent in custom preprocessor macros.

## Examples

Bad:

```cpp
// Bad: Manual if/if-else branching
if (cm->width > cpi->initial_width) cm->width = cpi->initial_width;
if (val < min_val) val = min_val; else if (val > max_val) val = max_val;

// Bad: Local macro or nested ternary expression
#define CLAMP(X, L, H) ((X) < (L) ? (L) : ((X) > (H) ? (H) : (X)))
int bounded = (val < low) ? low : ((val > high) ? high : val);
```

Good:

```cpp
#include "vpx_dsp/vpx_dsp_common.h"

// Good: Standard VPXMIN, VPXMAX, and clamp helpers
cm->width = VPXMIN(cm->width, cpi->initial_width);
val = clamp(val, min_val, max_val);

// Good: Shared inline clamp helper instead of macro/ternary
int bounded = clamp(val, low, high);
```
