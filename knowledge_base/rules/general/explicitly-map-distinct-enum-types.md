---
name: libvpx/explicitly-map-distinct-enum-types
description: When returning or converting values between distinct enum types, explicitly map or translate enumerators rather than relying on implicit enum coercion
rubric: readability
severity: should_fix
---

**Rule.** Do not implicitly convert, return, or assign values across distinct
enum types (such as returning a vpx_rc_status_t from a function returning
vpx_codec_err_t). Explicitly map or translate between different enum types using
conditional checks (such as ternary operators), switch statements, or dedicated
mapping lookup tables.

**Why.** Implicit conversion between distinct enum types violates type safety,
triggers compiler diagnostics (such as -Wenum-conversion), and dangerously
relies on accidental numeric equivalence between independently defined
enumerations. Explicit translation enforces type boundaries and ensures changes
to one enum definition do not silently break another.

## Examples

Bad:

```cpp
vpx_codec_err_t vp9_extrc_get_key_frame_decision(
    EXT_RATECTRL *ext_ratectrl,
    vpx_rc_key_frame_decision_t *key_frame_decision) {
  // Implicitly returning vpx_rc_status_t in a vpx_codec_err_t function.
  vpx_rc_status_t rc_status = ext_ratectrl->funcs.get_key_frame_decision(
      ext_ratectrl->model, key_frame_decision);
  return rc_status;
}
```

Good:

```cpp
vpx_codec_err_t vp9_extrc_get_key_frame_decision(
    EXT_RATECTRL *ext_ratectrl,
    vpx_rc_key_frame_decision_t *key_frame_decision) {
  // Explicitly translate the subsystem status enum to the codec error enum.
  vpx_rc_status_t rc_status = ext_ratectrl->funcs.get_key_frame_decision(
      ext_ratectrl->model, key_frame_decision);
  return rc_status == VPX_RC_OK ? VPX_CODEC_OK : VPX_CODEC_ERROR;
}
```
