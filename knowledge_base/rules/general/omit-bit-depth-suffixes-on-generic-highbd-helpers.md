---
name: libvpx/omit-bit-depth-suffixes-on-generic-highbd-helpers
description: When defining high-bitdepth helpers operating generically on 16-bit sample buffers across all bit depths, omit specific bit-depth suffixes like _8_ or _10_
rubric: readability
severity: should_fix
---

**Rule.** Do not include specific bit-depth suffixes (such as _8_, _10_, or
_12_) in high-bitdepth helper function names when the logic operates generically
on 16-bit buffers across all supported bit depths without bit-depth-specific
scaling or shifts. Reserve bit-depth qualifiers for routines whose computation
or scaling explicitly depends on the configured bit depth.

**Why.** In VP9 high-bitdepth mode, source buffers store 16-bit samples for all
bit depths (8, 10, and 12 bits). Suffixing a generic 16-bit helper with a
specific bit depth like _8_ is misleading because it implies the routine only
supports 8-bit input or performs 8-bit-specific operations.

## Examples

Bad:

```cpp
// Suffixing generic 16-bit buffer calculation with specific bit depth '_8_':
static void aq_highbd_8_variance(const uint8_t *a8, int a_stride,
                                 const uint8_t *b8, int b_stride, int w, int h,
                                 uint64_t *sse, int64_t *sum) {
  const uint16_t *a = CONVERT_TO_SHORTPTR(a8);
  const uint16_t *b = CONVERT_TO_SHORTPTR(b8);
  // Unscaled 16-bit variance computation across all bit depths...
}
```

Good:

```cpp
// Generic high-bitdepth helper named without misleading bit-depth suffix:
static void aq_highbd_variance64(const uint8_t *a8, int a_stride,
                                 const uint8_t *b8, int b_stride, int w, int h,
                                 uint64_t *sse, int64_t *sum) {
  const uint16_t *a = CONVERT_TO_SHORTPTR(a8);
  const uint16_t *b = CONVERT_TO_SHORTPTR(b8);
  // Unscaled 16-bit variance computation across all bit depths...
}
```
