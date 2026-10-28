---
name: libvpx/check-and-propagate-return-status-codes
description: When invoking callbacks or helper functions that return status or error codes, check and propagate failure to callers rather than returning success unconditionally
rubric: correctness
severity: should_fix
---

**Rule.** When invoking a callback, function pointer, or fallible helper that
returns a status or error code (such as vpx_rc_status_t or vpx_codec_err_t),
verify the status and propagate or translate failure to the caller. Do not leave
the return value unchecked, discard the returned status, or unconditionally
return success (VPX_CODEC_OK).

**Why.** Ignoring status or error codes returned by callbacks or subroutines
masks internal failures, prevents callers from detecting invalid states or
out-of-memory conditions, and leads to silent data corruption or undefined
behavior.

## Examples

Bad:

```cpp
vpx_rc_status_t rc_status = ext_ratectrl->funcs.get_key_frame_decision(
    ext_ratectrl->model, key_frame_decision);
return VPX_CODEC_OK;
```

Good:

```cpp
vpx_rc_status_t rc_status = ext_ratectrl->funcs.get_key_frame_decision(
    ext_ratectrl->model, key_frame_decision);
return rc_status == VPX_RC_OK ? VPX_CODEC_OK : VPX_CODEC_ERROR;
```
