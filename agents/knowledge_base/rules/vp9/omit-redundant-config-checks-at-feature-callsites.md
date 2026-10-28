---
name: libvpx/vp9/omit-redundant-config-checks-at-feature-callsites
description: When guarding feature execution in encoder routines, check only the centralized feature flag instead of repeating redundant bit-depth or mode checks
rubric: readability
severity: should_fix
---

**Rule.** Do not repeat redundant format, profile, mode, or bit-depth checks at
feature callsites when encoder setup already validates these constraints and
disables the feature flag (such as cpi->use_skin_detection or speed features).
Gate execution at callsites solely with the centralized feature enable flag.

**Why.** Centralizing configuration constraints in encoder initialization
ensures validation logic remains in one place, prevents drift across callsites,
and avoids cluttering inner-loop code with redundant runtime checks.

## Examples

Bad:

```cpp
// Redundant bit_depth check when use_skin_detection is already disabled for high bit-depth.
if (cpi->use_skin_detection && cm->bit_depth == VPX_BITS_8) {
  vp9_compute_skin_sb(cpi, BLOCK_16X16, mi_row, mi_col);
}
```

Good:

```cpp
// Encoder setup (in vp9_encoder.c) already disables use_skin_detection when unsupported.
if (cpi->use_skin_detection) {
  vp9_compute_skin_sb(cpi, BLOCK_16X16, mi_row, mi_col);
}
```
