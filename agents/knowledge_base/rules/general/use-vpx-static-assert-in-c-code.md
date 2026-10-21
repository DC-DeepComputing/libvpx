---
name: libvpx/use-vpx-static-assert-in-c-code
rubric: correctness
severity: blocking
---

**Rule.** Do not use C11-specific _Static_assert or static_assert in C source or
header files; use the portable VPX_STATIC_ASSERT macro from
vpx_ports/static_assert.h instead. Additionally, pass only integer constant
expressions to VPX_STATIC_ASSERT, and never include string literal messages
(such as cond && "msg").

**Why.** libvpx targets C89/gnu89 across diverse compiler toolchains, so C11
assertion keywords cause build failures on pre-C11 compilers. Because
VPX_STATIC_ASSERT relies on array or bitfield dimensions that require integer
constant expressions, passing string literals fails on MSVC with constant
expression compiler errors (C2057).

## Examples

Bad:

```cpp
#include <assert.h>

// Bad: C11 _Static_assert with string message
_Static_assert(TICKS_PER_SEC > 1000000 && (TICKS_PER_SEC % 1000000) == 0,
               "Unexpected TICKS_PER_SEC");

// Bad: String literal inside VPX_STATIC_ASSERT
VPX_STATIC_ASSERT(TICKS_PER_SEC > 1000000 &&
                  (TICKS_PER_SEC % 1000000) == 0 &&
                  "Unexpected TICKS_PER_SEC");
```

Good:

```cpp
#include "vpx_ports/static_assert.h"

VPX_STATIC_ASSERT(TICKS_PER_SEC > 1000000 &&
                  (TICKS_PER_SEC % 1000000) == 0);
```
