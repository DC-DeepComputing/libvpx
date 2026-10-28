---
name: libvpx/consolidate-duplicate-constant-tables
description: When defining constant tables or coefficient arrays shared across files, codecs, or SIMD implementations, consolidate them into a shared module or parent header
rubric: design
severity: should_fix
---

**Rule.** Avoid duplicating identical constant lookup tables, coefficient
arrays, filter matrices, or SIMD helper constants across multiple source files,
codec modules, or architecture subdirectories. Consolidate shared constant
tables into a single definition within a common component (such as vpx_dsp,
vpx_util, or a shared parent header like vp9/encoder/*_constants.h) and
reference them via external linkage or a shared header rather than duplicating
definitions across architecture subtrees (e.g., x86/, arm/neon/).

**Why.** Duplicating constant lookup tables and coefficient arrays across source
files or architecture subtrees wastes binary size, creates maintenance overhead,
and risks logic desynchronization or accidental divergence when values are
updated. Centralizing constants in a single shared header or module ensures all
codec modules and architecture ports share identical tables and simplifies
adding new optimizations.

## Examples

Bad:

```cpp
// Variant 1: Duplicating identical tables across codec/dsp modules
// In vp9/common/vp9_postproc.c
const int16_t vp9_rv[] = {
  8, 5, 2, 2, 8, 12, 4, 9, 8, 3,
  0, 3, 9, 0, 0, 0, 8, 3, 14, 4,
  // ... duplicated copy of identical table in vpx_dsp/deblock.c ...
};

// Variant 2: Duplicating SIMD constants across architecture subdirectories
// In vp9/encoder/arm/neon/vp9_temporal_filter_constants.h (duplicated from x86/)
#define NEIGHBOR_CONSTANT_4 (int16_t)49152
DECLARE_ALIGNED(16, static const int16_t,
                LEFT_CORNER_NEIGHBORS_PLUS_1[8]) = { 49152, 49152, ... };
```

Good:

```cpp
// Variant 1: Reference shared table from common module (e.g. vpx_dsp)
// In vp9/common/vp9_postproc.c
extern const int16_t vpx_rv[];

const int16_t *rv3 = &vpx_rv[63 & rand()];

// Variant 2: Consolidate SIMD constants in parent subsystem header
// In vp9/encoder/vp9_temporal_filter_constants.h (shared by x86 and arm)
#define NEIGHBOR_CONSTANT_4 (int16_t)49152
DECLARE_ALIGNED(16, static const int16_t,
                LEFT_CORNER_NEIGHBORS_PLUS_1[8]) = { 49152, 49152, ... };

// In vp9/encoder/arm/neon/vp9_temporal_filter_neon.c
#include "vp9/encoder/vp9_temporal_filter_constants.h"
```
