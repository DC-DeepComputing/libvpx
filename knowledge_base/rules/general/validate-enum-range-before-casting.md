---
name: libvpx/validate-enum-range-before-casting
description: When converting integers or strings to enum types, validate values before casting, using range checks for contiguous enums and switch or lookup tables for sparse enums
rubric: safety
severity: should_fix
---

**Rule.** When parsing configuration inputs or converting raw integer values to
an enum type, validate that the value is a defined enumerator before casting and
assignment. For contiguous/dense enums (e.g. `enum vpx_rc_mode`), validate using
minimum and maximum range bounds (e.g. `>= VPX_VBR && <= VPX_Q`). For sparse,
non-contiguous, or bitmask-composed enums (e.g. `vpx_img_fmt_t`), do not use
min/max range comparisons (`<`, `>`, `<=`, `>=`); validate instead using an
explicit `switch` statement, lookup table, or dedicated validator function
(`is_valid_img_fmt`). Reject or report invalid values through normal
error-handling control flow rather than casting out-of-range integers.

**Why.** Directly casting unvalidated integers or strings to an enum type can
produce out-of-range values that cause undefined behavior, invalid internal
states, or downstream logic bugs. Furthermore, using range comparisons (`<`,
`<=`, `>`, `>=`) on sparse or bitmask enums (like `vpx_img_fmt_t`) is unsafe
because undefined intermediate gap values pass range checks undetected.

## Examples

Bad:

```cpp
// Bad 1: Casting raw parsed config directly without bounds validation
if (strcmp(config.name, "rc_mode") == 0) {
  oxcf->rc_mode = (enum vpx_rc_mode)atoi(config.value);
  return 1;
}

// Bad 2: Range check on sparse/bitmask enum (VPX_IMG_FMT_* has gaps)
if (fmt <= VPX_IMG_FMT_NONE || fmt > VPX_IMG_FMT_I44016) {
  return VPX_CODEC_INVALID_PARAM;
}
```

Good:

```cpp
// Good 1: Validate contiguous enum against min/max bounds before casting
if (strcmp(config.name, "rc_mode") == 0) {
  int rc_mode = atoi(config.value);
  if (rc_mode >= VPX_VBR && rc_mode <= VPX_Q) {
    oxcf->rc_mode = (enum vpx_rc_mode)rc_mode;
    return 1;
  }
  fprintf(stderr, "Invalid rc_mode value: %d\n", rc_mode);
  return 0;
}

// Good 2: Validate sparse/bitmask enum with switch or validator function
static int is_valid_img_fmt(vpx_img_fmt_t fmt) {
  switch (fmt) {
    case VPX_IMG_FMT_YV12:
    case VPX_IMG_FMT_I420:
    case VPX_IMG_FMT_I422:
    case VPX_IMG_FMT_I444:
    case VPX_IMG_FMT_I440:
    case VPX_IMG_FMT_NV12:
    case VPX_IMG_FMT_I42016:
    case VPX_IMG_FMT_I42216:
    case VPX_IMG_FMT_I44416:
    case VPX_IMG_FMT_I44016: return 1;
    default: return 0;
  }
}
```
