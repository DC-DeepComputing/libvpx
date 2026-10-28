---
name: libvpx/pass-dummy-values-for-unused-callback-parameters
description: When invoking callbacks or helper APIs in contexts where parameters are unused, pass a fixed dummy value instead of duplicating calculation logic
rubric: design
severity: should_fix
---

**Rule.** Do not duplicate complex parameter calculation logic or copy helper
functions when invoking callbacks, external controllers, or helper APIs in
contexts where those arguments are unused or ignored by the callee. Pass an
explicit fixed default value or dummy flag instead, and document with a comment
why the fixed value is sufficient.

**Why.** Duplicating non-trivial parameter calculation logic to produce values
that are ignored in auxiliary passes creates dead-code maintenance overhead. It
also confuses readers by falsely implying that the calculated value affects the
operation.

## Examples

Bad:

```cpp
static int get_ref_frame_flags(const VP9_COMP *cpi) {
  // Complex duplicated logic to determine active reference frame flags...
  return flags;
}

// In an auxiliary analysis pass where the external controller ignores flags:
const int ref_frame_flags = get_ref_frame_flags(cpi);
codec_status = vp9_extrc_get_encodeframe_decision(
    &cpi->ext_ratectrl, frame_index, ref_frame_flags, &decision);
```

Good:

```cpp
// When reference flags are unused by the external controller during
// this auxiliary pass, pass fixed dummy flags rather than copying logic.
const int ref_frame_flags = VP9_ALT_FLAG | VP9_GOLD_FLAG | VP9_LAST_FLAG;
codec_status = vp9_extrc_get_encodeframe_decision(
    &cpi->ext_ratectrl, frame_index, ref_frame_flags, &decision);
```
