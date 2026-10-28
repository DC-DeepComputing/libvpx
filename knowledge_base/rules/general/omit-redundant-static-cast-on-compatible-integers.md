---
name: libvpx/omit-redundant-static-cast-on-compatible-integers
description: When initializing variables, arrays, or passing arguments with compatible integer types, enums, or bitwise flag combinations, omit redundant static_cast expressions.
rubric: readability
severity: nit
---

**Rule.** Avoid redundant or unnecessary static_cast expressions when
initializing variables, array elements, or passing arguments from compatible
integer types, enums, or bitwise flag operations. Rely on standard implicit
integral conversions and promotions where types are compatible and conversions
are lossless and warning-free.

**Why.** Implicit conversion cleanly handles compatible integer types, enums,
and bitmask constants without compiler diagnostics. Explicit casts add visual
noise and make initialization lists harder to maintain.

## Examples

Bad:

```cpp
const vpx_enc_frame_flags_t frame_flags[3] = {
  VPX_EFLAG_FORCE_KF,
  static_cast<vpx_enc_frame_flags_t>(VP8_EFLAG_NO_REF_LAST |
                                     VP8_EFLAG_NO_REF_GF),
  0
};
```

Good:

```cpp
static constexpr std::array<vpx_enc_frame_flags_t, 3> frame_flags = {
  VPX_EFLAG_FORCE_KF, VP8_EFLAG_NO_REF_LAST | VP8_EFLAG_NO_REF_GF, 0
};
```
