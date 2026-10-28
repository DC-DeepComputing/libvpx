---
name: libvpx/exact-struct-member-names-in-initializer-comments
description: When annotating positional struct initializers with inline field comments, use the exact declared struct member identifiers rather than expanding abbreviations or paraphrasing names.
rubric: readability
severity: nit
---

**Rule.** When annotating positional struct initializers or configuration tables
with inline comments to identify initialized fields, use the exact struct member
identifiers from the struct declaration. Do not expand abbreviations (e.g.,
writing `threshold` instead of `thresh`), paraphrase field names, or use
descriptive English variations in the comments.

**Why.** Positional aggregate initializers rely on inline comments for
readability and field mapping. Using exact member identifiers preserves grep and
symbol searchability across the codebase when locating field initializations,
prevents confusion with similarly named fields, and keeps configuration tables
consistent with the canonical struct definitions.

## Examples

Bad:

```cpp
static const vpx_codec_enc_cfg_t default_cfg = {
  /* ... */
  60, /* rc_resize_down_threshold */
  30, // rc_resize_up_threshold
  VPX_VBR, /* rc_end_usage */
};
```

Good:

```cpp
static const vpx_codec_enc_cfg_t default_cfg = {
  /* ... */
  60, /* rc_resize_down_thresh */
  30, // rc_resize_up_thresh
  VPX_VBR, /* rc_end_usage */
};
```
