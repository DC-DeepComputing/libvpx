---
name: libvpx/pass-null-for-generic-vpx-internal-error-detail
description: When calling vpx_internal_error without specific failure diagnostics, pass NULL for the detail format string rather than repeating generic error descriptions
rubric: readability
severity: nit
---

**Rule.** When calling vpx_internal_error(), pass NULL as the detail format
string unless specific diagnostic details (such as unexpected dimensions,
counts, or invalid token values) are available. Do not pass generic string
literals that merely duplicate standard error descriptions already provided by
vpx_codec_err_to_string() (such as 'Corrupted frame detected' for
VPX_CODEC_CORRUPT_FRAME).

**Why.** vpx_codec_err_to_string() already provides standard human-readable
descriptions for each vpx_codec_err_t error code. Passing generic strings that
duplicate the enum description adds redundant string literals to the binary and
sets has_detail without providing actionable diagnostic value.

## Examples

Bad:

```cpp
// Redundant: repeats the standard message for VPX_CODEC_CORRUPT_FRAME
vpx_internal_error(&pbi->common.error, VPX_CODEC_CORRUPT_FRAME,
                   "Corrupted frame detected");
```

Good:

```cpp
// Pass NULL when no additional diagnostic context is available
vpx_internal_error(&pbi->common.error, VPX_CODEC_CORRUPT_FRAME, NULL);

// Or provide specific diagnostic context with formatting
vpx_internal_error(&pbi->common.error, VPX_CODEC_CORRUPT_FRAME,
                   "Invalid partition size %d at offset %d", size, offset);
```
