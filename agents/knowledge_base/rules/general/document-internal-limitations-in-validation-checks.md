---
name: libvpx/document-internal-limitations-in-validation-checks
description: When rejecting inputs or configurations due to internal implementation limitations rather than format incompatibilities, add an explanatory comment detailing the limitation
rubric: readability
severity: nit
---

**Rule.** When adding validation checks or rejecting user configurations due to
internal architectural or library limitations (rather than format or
specification constraints), add an explanatory comment detailing why the
restriction exists. Document what internal state, function pointer assignment,
or subsystem limitation requires the check, clarifying what needs refactoring to
support dynamic configuration later.

**Why.** Without explanatory comments, maintainers cannot distinguish between
inherent specification constraints and temporary internal architecture
limitations. Documenting the underlying limitation clarifies why valid
configuration combinations are rejected and guides future refactoring to enable
dynamic support.

## Examples

Bad:

```cpp
#if CONFIG_VP9_HIGHBITDEPTH
  if (ctx->oxcf.use_highbitdepth && !(img->fmt & VPX_IMG_FMT_HIGHBITDEPTH)) {
    ERROR(
        "Encoder initialized with VPX_CODEC_USE_HIGHBITDEPTH requires a "
        "high-bit-depth image format (e.g. VPX_IMG_FMT_I42016)");
  }
#endif
```

Good:

```cpp
#if CONFIG_VP9_HIGHBITDEPTH
  // The following checks ensure that the image format matches the encoder's
  // high bit-depth setting. Currently, libvpx overwrites function pointers if
  // use_highbitdepth is set to 1, but there is no other way around if the
  // internal flag gets reset to 0.
  if (ctx->oxcf.use_highbitdepth && !(img->fmt & VPX_IMG_FMT_HIGHBITDEPTH)) {
    ERROR(
        "Encoder initialized with VPX_CODEC_USE_HIGHBITDEPTH requires a "
        "high-bit-depth image format (e.g. VPX_IMG_FMT_I42016)");
  }
#endif
```
