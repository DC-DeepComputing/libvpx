---
name: libvpx/use-vpx-integer-header-for-standard-integers
description: Include "vpx/vpx_integer.h" (or "./vpx_integer.h" in public vpx headers) for fixed-width integer types and format macros, and omit redundant <stdint.h>, <inttypes.h>, or <stddef.h> includes
rubric: correctness
severity: should_fix
---

**Rule.** Include "vpx/vpx_integer.h" (or #include "./vpx_integer.h" in public
headers under vpx/) rather than standard library headers <stdint.h>,
<inttypes.h>, or <stddef.h> when referencing fixed-width integer types (uint8_t,
int16_t, int32_t, int64_t, uintptr_t, intptr_t), limits (INT64_MAX, UINT64_MAX),
or format/scan macros (PRId64, PRIu64). Do not directly or redundantly include
<stdint.h>, <inttypes.h>, or <stddef.h> alongside vpx_integer.h in libvpx source
or header files.

**Why.** "vpx/vpx_integer.h" acts as libvpx's standard portability wrapper that
includes <stddef.h>, <inttypes.h>, and <stdint.h> while ensuring format/limit
macros (__STDC_FORMAT_MACROS, __STDC_LIMIT_MACROS) and inlining definitions
(VPX_INLINE, VPX_FORCE_INLINE) are defined across all supported compilers (such
as MSVC, C90, and C++). Directly including <stdint.h>, <inttypes.h>, or
<stddef.h> bypasses these compatibility definitions or creates redundant header
clutter when vpx_integer.h is already included.

## Examples

Bad:

```cpp
// Variant 1: Direct stdint/inttypes includes without vpx_integer.h
#include <stdint.h>
#include <inttypes.h>
void process_block(uint32_t size, int64_t offset);

// Variant 2: Redundant stdint/stddef when vpx_integer.h is present
#include <stdint.h>
#include <stdlib.h>
#include "vpx/vpx_integer.h"

// Variant 3: Public header under vpx/ using stdint.h instead of ./vpx_integer.h
#include <stdint.h>
typedef struct VpxBlockStats { int16_t row; int16_t col; } VpxBlockStats;
```

Good:

```cpp
// Variant 1: Using vpx_integer.h for fixed-width types
#include "vpx/vpx_integer.h"
void process_block(uint32_t size, int64_t offset);

// Variant 2: Omit redundant stdint.h when vpx_integer.h is included
#include <stdlib.h>
#include "vpx/vpx_integer.h"

// Variant 3: Public header under vpx/ uses relative ./vpx_integer.h
#include "./vpx_integer.h"
typedef struct VpxBlockStats { int16_t row; int16_t col; } VpxBlockStats;
```
