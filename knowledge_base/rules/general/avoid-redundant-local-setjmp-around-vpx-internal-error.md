---
name: libvpx/avoid-redundant-local-setjmp-around-vpx-internal-error
description: When reporting local errors with vpx_internal_error or calling non-jumping helpers, avoid redundant local setjmp blocks; clean up and return directly
rubric: readability
severity: should_fix
---

**Rule.** Do not wrap local calls to vpx_internal_error() or non-jumping
functions in redundant local setjmp() blocks. When error.setjmp (or
info->setjmp) is 0, vpx_internal_error() records the error status in struct
vpx_internal_error_info and returns normally; perform direct local cleanup and
return the error code. Additionally, do not set up setjmp recovery blocks around
helper functions that indicate failure via return values or sentinel status
codes (e.g. INVALID_IDX or NULL) without invoking longjmp().

**Why.** vpx_internal_error() only invokes longjmp() when info->setjmp is
non-zero. Setting up a local setjmp block immediately before calling
vpx_internal_error() or wrapping helper functions that signal errors via return
values introduces unnecessary control flow complexity, extra setjmp execution
overhead on success paths, and obscures error handling paths.

## Examples

Bad:

```cpp
// Variant 1: Wrapping non-jumping helper function in setjmp
if (setjmp(cm->error.jmp)) {
  cm->error.setjmp = 0;
  release_fb_on_decoder_exit(pbi);
  return -1;
}
cm->error.setjmp = 1;
cm->new_fb_idx = get_free_fb(cm);

// Variant 2: Redundant local setjmp immediately around vpx_internal_error
if (setjmp(cm->error.jmp)) {
  cm->error.setjmp = 0;
  release_fb_on_decoder_exit(pbi);
  return -1;
}
cm->error.setjmp = 1;
vpx_internal_error(&cm->error, VPX_CODEC_MEM_ERROR, "No free fb");
```

Good:

```cpp
// Variant 1: Check return value directly without wrapping in setjmp
cm->new_fb_idx = get_free_fb(cm);
if (cm->new_fb_idx == INVALID_IDX) {
  release_fb_on_decoder_exit(pbi);
  vpx_internal_error(&cm->error, VPX_CODEC_MEM_ERROR, "No free fb");
  return -1;
}

// Variant 2: Clean up directly and return when setjmp is disabled
release_fb_on_decoder_exit(pbi);
vpx_internal_error(&cm->error, VPX_CODEC_MEM_ERROR, "No free fb");
return cm->error.error_code;
```
