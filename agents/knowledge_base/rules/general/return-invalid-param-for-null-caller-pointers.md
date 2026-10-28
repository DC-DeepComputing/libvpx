---
name: libvpx/return-invalid-param-for-null-caller-pointers
description: In functions returning vpx_codec_err_t and codec controls, return VPX_CODEC_INVALID_PARAM for NULL/invalid pointers and VPX_CODEC_MEM_ERROR on allocation failure rather than generic VPX_CODEC_ERROR or setjmp
rubric: safety
severity: should_fix
---

**Rule.** When implementing functions that return vpx_codec_err_t (including
public API entry points, codec controls, and internal status helpers), return
specific error codes rather than generic VPX_CODEC_ERROR:

1. Validate required input pointer parameters and caller-provided structures against NULL before dereferencing and return VPX_CODEC_INVALID_PARAM (not VPX_CODEC_MEM_ERROR or VPX_CODEC_ERROR). In public header Doxygen comments, document VPX_CODEC_INVALID_PARAM for NULL pointer arguments.
2. When dynamic memory allocation (via vpx_malloc, vpx_calloc, vpx_memalign, or vpx_realloc) fails, directly check the pointer (e.g., `if (!ptr)`) and return VPX_CODEC_MEM_ERROR (not generic VPX_CODEC_ERROR). Do not set up local setjmp error handlers or use CHECK_MEM_ERROR (which performs a longjmp via vpx_internal_error) in functions designed to return status error codes.

**Why.** Returning specific error codes (VPX_CODEC_INVALID_PARAM,
VPX_CODEC_MEM_ERROR) provides precise diagnostic information to callers and test
suites, distinguishing caller-side argument bugs from internal system
out-of-memory allocation failures. Returning or documenting VPX_CODEC_MEM_ERROR
for NULL pointer arguments misleads callers into treating invalid parameters as
transient memory shortages. Directly checking allocation pointers and
propagating VPX_CODEC_MEM_ERROR maintains structured, explicit control flow
without the unnecessary complexity and nonlocal jump overhead of setjmp/longjmp
exception handling.

## Examples

Bad:

```cpp
// Variant 1: Missing NULL check or returning MEM_ERROR / generic ERROR for NULL argument
vpx_codec_err_t ctrl_get_tpl_stats(vpx_codec_alg_priv_t *ctx, va_list args) {
  VpxTplGopStats *tpl_stats = va_arg(args, VpxTplGopStats *);
  if (tpl_stats == NULL) return VPX_CODEC_MEM_ERROR;  // Wrong status for invalid arg
  return get_tpl_stats(ctx, tpl_stats);
}

// Variant 2: Returning generic ERROR or using local setjmp/CHECK_MEM_ERROR on malloc failure
vpx_codec_err_t vp9_extrc_init(EXT_RATECTRL *ext_ratectrl) {
  if (ext_ratectrl == NULL) return VPX_CODEC_ERROR;
  ext_ratectrl->model = vpx_malloc(sizeof(*ext_ratectrl->model));
  if (ext_ratectrl->model == NULL) return VPX_CODEC_ERROR;  // Generic error on allocation failure
  return VPX_CODEC_OK;
}
```

Good:

```cpp
// Variant 1: Return INVALID_PARAM for NULL caller pointers
vpx_codec_err_t ctrl_get_tpl_stats(vpx_codec_alg_priv_t *ctx, va_list args) {
  VpxTplGopStats *tpl_stats = va_arg(args, VpxTplGopStats *);
  if (tpl_stats == NULL) return VPX_CODEC_INVALID_PARAM;
  return get_tpl_stats(ctx, tpl_stats);
}

// Variant 2: Return INVALID_PARAM for bad args and MEM_ERROR on allocation failure
vpx_codec_err_t vp9_extrc_init(EXT_RATECTRL *ext_ratectrl) {
  if (ext_ratectrl == NULL) return VPX_CODEC_INVALID_PARAM;
  ext_ratectrl->model = vpx_malloc(sizeof(*ext_ratectrl->model));
  if (ext_ratectrl->model == NULL) return VPX_CODEC_MEM_ERROR;
  return VPX_CODEC_OK;
}
```
