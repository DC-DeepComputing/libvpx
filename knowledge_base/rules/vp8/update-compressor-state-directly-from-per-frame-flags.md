---
name: libvpx/vp8/update-compressor-state-directly-from-per-frame-flags
description: When handling per-frame API encode flags in encoder interfaces, update compressor state directly rather than threading redundant internal frame flags
rubric: design
severity: should_fix
---

**Rule.** In encoder interface wrappers (such as vp8_cx_iface.c and
vp9_cx_iface.c), update compressor instance state (cpi) directly from per-frame
API encode flags (e.g., VPX_EFLAG_CALCULATE_PSNR) during encode calls. Do not
introduce or thread intermediate internal frame flags (such as FRAMEFLAGS_*)
through lower-level codec core functions when the compressor state can be
configured directly in the interface entry point.

**Why.** The encoder interface entry point is called once per frame with the
complete API flag set, allowing per-frame flags to directly configure compressor
instance fields without cluttering core compression interfaces. This eliminates
redundant flag translations, simplifies internal call chains, and maintains
architectural uniformity between VP8 and VP9/AV1 codec interfaces.

## Examples

Bad:

```cpp
/* Introducing an intermediate internal frame flag threaded to compressor */
lib_flags = (flags & VPX_EFLAG_FORCE_KF) ? FRAMEFLAGS_KEY : 0;
if ((flags & VPX_EFLAG_CALCULATE_PSNR) &&
    !(ctx->base.init_flags & VPX_CODEC_USE_PSNR)) {
  lib_flags |= FRAMEFLAGS_PSNR;
}
```

Good:

```cpp
/* Directly update the compressor struct field on each encode call */
((VP8_COMP *)ctx->cpi)->b_calculate_psnr =
    (ctx->base.init_flags & VPX_CODEC_USE_PSNR) ||
    (flags & VPX_EFLAG_CALCULATE_PSNR);
```
