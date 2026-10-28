---
name: libvpx/vp8/place-shared-codec-utilities-in-vpx-util
description: When adding or modifying helper functions, math utilities, or data types shared across multiple codecs, place them in vpx_util/ instead of duplicating definitions.
rubric: design
severity: should_fix
---

**Rule.** Place utility functions, math helpers, and data structures that are
shared across multiple codec implementations (such as VP8, VP9, or
encoder/decoder interfaces) in vpx_util/ headers instead of duplicating them
across codec-specific source files. Include the shared vpx_util/ header in each
codec module that requires the utility.

**Why.** Duplicating common helper functions or math routines across
codec-specific directories increases maintenance overhead, risks implementation
drift or subtle divergence between VP8 and VP9, and complicates bug fixes.

## Examples

Bad:

```cpp
// In vp9/vp9_cx_iface.c (and duplicated in vp8/vp8_cx_iface.c)
static INLINE int gcd(int64_t a, int b) {
  int r;
  while (b > 0) {
    r = (int)(a % b);
    a = b;
    b = r;
  }
  return (int)a;
}

static INLINE void reduce_ratio(vpx_rational64_t *ratio) {
  const int denom = gcd(ratio->num, ratio->den);
  ratio->num /= denom;
  ratio->den /= denom;
}
```

Good:

```cpp
// In vpx_util/vpx_timestamp.h
static INLINE int gcd(int64_t a, int b) {
  int r;
  while (b > 0) {
    r = (int)(a % b);
    a = b;
    b = r;
  }
  return (int)a;
}

static INLINE void reduce_ratio(vpx_rational64_t *ratio) {
  const int denom = gcd(ratio->num, ratio->den);
  ratio->num /= denom;
  ratio->den /= denom;
}

// In vp9/vp9_cx_iface.c and vp8/vp8_cx_iface.c
#include "vpx_util/vpx_timestamp.h"
```
