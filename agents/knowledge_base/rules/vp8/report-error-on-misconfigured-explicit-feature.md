---
name: libvpx/vp8/report-error-on-misconfigured-explicit-feature
description: When an optional or external subsystem feature is explicitly enabled, return an error rather than silently falling back if required prerequisites or callbacks are missing
rubric: correctness
severity: should_fix
---

**Rule.** When an optional or external subsystem feature (such as external rate
control or custom quantization) is explicitly enabled by configuration, verify
that all required prerequisite callbacks and configuration parameters are valid.
If required setup is missing or incomplete, report a fatal error (e.g., using
`vpx_internal_error(&cm->error, VPX_CODEC_INVALID_PARAM, ...)` or returning an
error status) rather than silently ignoring the setting and falling back to
default behavior.

**Why.** When a user or client application explicitly enables an optional
feature or custom subsystem mode, missing prerequisites indicate a configuration
bug. Silently falling back to default behavior hides integration errors and
produces unexpected codec output, whereas failing explicitly alerts callers
immediately.

## Examples

Bad:

```cpp
if (cpi->tpl_with_external_rc) {
  if (cpi->ext_ratectrl.ready &&
      (cpi->ext_ratectrl.funcs.rc_type & VPX_RC_QP) != 0 &&
      cpi->ext_ratectrl.funcs.get_encodeframe_decision != NULL) {
    // Run external rate control for TPL...
  }
  // Silently falling back to internal rate control on incomplete setup
}
```

Good:

```cpp
if (cpi->tpl_with_external_rc) {
  if (cpi->ext_ratectrl.ready &&
      (cpi->ext_ratectrl.funcs.rc_type & VPX_RC_QP) != 0 &&
      cpi->ext_ratectrl.funcs.get_encodeframe_decision != NULL) {
    // Run external rate control for TPL...
  } else {
    vpx_internal_error(&cm->error, VPX_CODEC_INVALID_PARAM,
                       "The external rate control library is not set "
                       "properly for TPL pass.");
  }
}
```
