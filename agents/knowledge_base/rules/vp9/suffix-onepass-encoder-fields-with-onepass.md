---
name: libvpx/vp9/suffix-onepass-encoder-fields-with-onepass
description: When adding encoder struct fields, flags, or calculations specific to one-pass mode, append a _onepass suffix to clarify pass exclusivity
rubric: readability
severity: should_fix
---

**Rule.** When declaring struct members, configuration flags, or helper
functions that apply exclusively to one-pass encoding, append an explicit
_onepass suffix (such as compute_source_sad_onepass or fixed_qp_onepass). Do not
use unadorned field names for pass-restricted logic when the encoder context is
shared across one-pass and multi-pass modes.

**Why.** Encoder structs like VP9_COMP and VP9_SPEED_FEATURES are shared across
both one-pass and two-pass encoding pipelines. Suffixing pass-specific fields
with _onepass makes their operational scope explicit to readers, prevents
accidental reliance on uncalculated state during two-pass modes, and preserves
naming consistency across encoder structs.

## Examples

Bad:

```cpp
// In VP9_COMP or VP9_SPEED_FEATURES struct definition:
int compute_frame_low_motion;
int fixed_qp;

// In encoder pipeline:
if (cpi->compute_frame_low_motion && oxcf->pass == 0) {
  // Pass-specific calculation
}
```

Good:

```cpp
// In VP9_COMP or VP9_SPEED_FEATURES struct definition:
int compute_frame_low_motion_onepass;
int fixed_qp_onepass;

// In encoder pipeline:
if (cpi->compute_frame_low_motion_onepass && oxcf->pass == 0) {
  // Pass-specific calculation
}
```
