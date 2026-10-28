---
name: libvpx/no-vpx-internal-error-in-cleanup
description: When cleaning up or deallocating resources in teardown functions, do not call vpx_internal_error() because longjmp aborts execution and leaks resources
rubric: safety
severity: should_fix
---

**Rule.** Do not invoke vpx_internal_error() or macros that trigger internal
error jumps inside destructor, teardown, or resource deallocation functions
(such as *_destroy, *_remove_*, *_free_*, or dealloc_*). Handle or ignore
non-fatal deallocation failures locally and complete all subsequent resource
cleanup steps.

**Why.** vpx_internal_error() performs a longjmp() when setjmp is configured on
the codec error info. Calling it inside cleanup routines abruptly terminates
function execution, skipping subsequent deallocation statements and permanently
leaking allocated memory and resources.

## Examples

Bad:

```cpp
vpx_codec_err_t codec_status = vp9_extrc_delete(&cpi->ext_ratectrl);
if (codec_status != VPX_CODEC_OK) {
  vpx_internal_error(&cm->error, codec_status, "vp9_extrc_delete() failed");
}

vp9_remove_common(cm);
vpx_free(cpi);
```

Good:

```cpp
vp9_extrc_delete(&cpi->ext_ratectrl);

vp9_remove_common(cm);
vpx_free(cpi);
```
