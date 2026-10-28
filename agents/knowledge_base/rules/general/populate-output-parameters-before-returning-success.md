---
name: libvpx/populate-output-parameters-before-returning-success
description: When returning success status codes from functions with pointer out-parameters, ensure output arguments are populated, or assert preconditions or return an error code.
rubric: correctness
severity: should_fix
---

**Rule.** Do not return a success status code (such as VPX_CODEC_OK) from a
function without populating its mandatory output parameters. If function
execution cannot proceed or required context is missing, either assert required
preconditions (if callers are obligated to satisfy them) or return an
appropriate error status code (such as VPX_CODEC_INVALID_PARAM or
VPX_CODEC_ERROR).

**Why.** Returning success without populating output pointer parameters causes
callers to consume uninitialized memory or garbage stack data under the
assumption that the function succeeded, leading to subtle logic bugs,
nondeterministic encoder decisions, or memory corruption.

## Examples

Bad:

```cpp
vpx_codec_err_t vp9_extrc_get_encodeframe_decision(
    EXT_RATECTRL *ext_ratectrl, int show_index, int coding_index,
    vpx_rc_encodeframe_decision_t *encode_frame_decision) {
  if (ext_ratectrl == NULL) return VPX_CODEC_INVALID_PARAM;
  if (ext_ratectrl->ready && (ext_ratectrl->funcs.rc_type & VPX_RC_QP) != 0) {
    return ext_ratectrl->funcs.get_encodeframe_decision(...) == VPX_RC_ERROR
               ? VPX_CODEC_ERROR
               : VPX_CODEC_OK;
  }
  return VPX_CODEC_OK;  // Returns success without setting *encode_frame_decision
}
```

Good:

```cpp
vpx_codec_err_t vp9_extrc_get_encodeframe_decision(
    EXT_RATECTRL *ext_ratectrl, int show_index, int coding_index,
    vpx_rc_encodeframe_decision_t *encode_frame_decision) {
  assert(ext_ratectrl != NULL);
  assert(ext_ratectrl->ready && (ext_ratectrl->funcs.rc_type & VPX_RC_QP) != 0);
  if (ext_ratectrl->funcs.get_encodeframe_decision(...) == VPX_RC_ERROR) {
    return VPX_CODEC_ERROR;
  }
  return VPX_CODEC_OK;
}
```
