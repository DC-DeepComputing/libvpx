---
name: libvpx/rename-parameter-for-volatile-copy
description: When creating a volatile local copy of a function parameter across setjmp calls, rename the incoming parameter rather than modifying variable usages throughout the function body
rubric: readability
severity: nit
---

**Rule.** When creating a local `volatile` copy of a function parameter (such as
to prevent clobbering across `setjmp`/`longjmp`), rename the incoming parameter
in the function signature and assign it to a `volatile` local variable retaining
the original identifier. Do not rename variable references throughout the entire
function body.

**Why.** Renaming the incoming parameter in the signature confines the edit to
the function entry, avoiding churn across the entire function body and
maintaining naming consistency across codec interfaces.

## Examples

Bad:

```cpp
static vpx_codec_err_t vp8e_encode(vpx_codec_alg_priv_t *ctx,
                                   const vpx_image_t *img,
                                   vpx_codec_pts_t pts,
                                   unsigned long duration,
                                   vpx_enc_frame_flags_t flags,
                                   unsigned long deadline) {
  volatile vpx_enc_frame_flags_t vflags = flags;
  // Modifying every occurrence of 'flags' to 'vflags' throughout the body:
  if (!vflags) {
    vflags = ctx->control_frame_flags;
  }
}
```

Good:

```cpp
static vpx_codec_err_t vp8e_encode(vpx_codec_alg_priv_t *ctx,
                                   const vpx_image_t *img,
                                   vpx_codec_pts_t pts,
                                   unsigned long duration,
                                   vpx_enc_frame_flags_t enc_flags,
                                   unsigned long deadline) {
  volatile vpx_enc_frame_flags_t flags = enc_flags;
  // Function body continues using original identifier 'flags' without churn:
  if (!flags) {
    flags = ctx->control_frame_flags;
  }
}
```
