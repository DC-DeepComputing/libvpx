---
name: libvpx/vp8/use-local-flags-variable-in-encoder-encode
description: When inspecting or modifying frame encoding flags in codec interface encode functions (encoder_encode or vp8e_encode), use the local volatile flags variable rather than enc_flags.
rubric: correctness
severity: blocking
---

**Rule.** In codec interface encode functions (encoder_encode in vp9_cx_iface.c
and vp8e_encode in vp8_cx_iface.c), do not reference the raw enc_flags
parameter. Reference and update the local working volatile vpx_enc_frame_flags_t
flags variable instead.

**Why.** In codec interface implementations, enc_flags is copied into a local
volatile flags variable at function entry to safely persist across setjmp
error-recovery boundaries and track dynamic intra-frame flag mutations (such as
forced keyframes or layer-specific flags). Referencing enc_flags directly
bypasses intermediate flag updates and can cause inconsistent encoder state or
stale flag evaluation.

## Examples

Bad:

```cpp
cpi->b_calculate_psnr = (ctx->base.init_flags & VPX_CODEC_USE_PSNR) ||
                        (enc_flags & VPX_EFLAG_CALCULATE_PSNR);
```

Good:

```cpp
cpi->b_calculate_psnr = (ctx->base.init_flags & VPX_CODEC_USE_PSNR) ||
                        (flags & VPX_EFLAG_CALCULATE_PSNR);
```
