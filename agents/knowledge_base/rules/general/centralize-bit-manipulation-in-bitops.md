---
name: libvpx/centralize-bit-manipulation-in-bitops
description: When performing low-level bit operations like finding least- or most-significant bits, use vpx_ports/bitops.h helpers instead of local compiler intrinsics or ad-hoc loops
rubric: design
severity: should_fix
---

**Rule.** Place shared low-level bit manipulation operations (such as finding
the least-significant or most-significant bit) in vpx_ports/bitops.h and use its
helpers (get_lsb(), get_msb()) across source files. Do not define ad-hoc,
file-local bit-scan functions or open-code compiler intrinsics (like
__builtin_ctz or _BitScanForward) directly in individual codec files.

**Why.** Centralizing bit operations in vpx_ports/bitops.h ensures consistent,
tested platform handling across compilers (GCC, Clang, MSVC) with portable
fallback paths, prevents code duplication, and avoids undefined behavior at zero
inputs.

## Examples

Bad:

```cpp
#if defined(_MSC_VER)
#include <intrin.h>
#pragma intrinsic(_BitScanForward)
static inline int bsf(int n) {
  unsigned long count;
  _BitScanForward(&count, n);
  return count;
}
#elif defined(__GNUC__)
static inline int bsf(int n) { return __builtin_ctz(n); }
#endif
```

Good:

```cpp
#include "vpx_ports/bitops.h"

// Use centralized port helpers that handle compiler intrinsics and fallbacks:
int first_set_bit = get_lsb(mask);
int last_set_bit = get_msb(mask);
```
