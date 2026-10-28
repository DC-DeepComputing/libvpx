---
name: libvpx/general-prefer-vpx-codec-init-macros-over-init-ver
description: When initializing encoder or decoder contexts, use the vpx_codec_enc_init or vpx_codec_dec_init convenience macros instead of explicitly calling *_init_ver with ABI version constants
rubric: readability
severity: nit
---

**Rule.** When initializing codec contexts in tests, examples, or client code,
use the convenience macros vpx_codec_enc_init(), vpx_codec_enc_init_multi(), and
vpx_codec_dec_init() rather than explicitly calling vpx_codec_enc_init_ver(),
vpx_codec_enc_init_multi_ver(), or vpx_codec_dec_init_ver() with ABI version
constants. Reserve direct *_init_ver() calls strictly for tests that
deliberately validate ABI version mismatch handling.

**Why.** The convenience macros automatically pass VPX_ENCODER_ABI_VERSION and
VPX_DECODER_ABI_VERSION, reducing boilerplate and preventing potential ABI
version constant mismatches at initialization call sites.

## Examples

Bad:

```cpp
// Explicitly passing VPX_ENCODER_ABI_VERSION to *_init_ver:
ASSERT_EQ(vpx_codec_enc_init_ver(&ctx, iface, &cfg, init_flags,
                                 VPX_ENCODER_ABI_VERSION),
          VPX_CODEC_OK);

// Explicitly calling decoder init_ver:
if (vpx_codec_dec_init_ver(&dec, iface, &dec_cfg, flags,
                           VPX_DECODER_ABI_VERSION) != VPX_CODEC_OK) {
  return -1;
}
```

Good:

```cpp
// Using convenience macros that automatically pass the ABI version:
ASSERT_EQ(vpx_codec_enc_init(&ctx, iface, &cfg, init_flags), VPX_CODEC_OK);

if (vpx_codec_dec_init(&dec, iface, &dec_cfg, flags) != VPX_CODEC_OK) {
  return -1;
}
```
