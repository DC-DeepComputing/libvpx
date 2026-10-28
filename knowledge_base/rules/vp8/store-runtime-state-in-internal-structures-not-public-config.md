---
name: libvpx/vp8/store-runtime-state-in-internal-structures-not-public-config
description: When updating encoder configuration or tracking frame-to-frame state in codec interfaces, store internal runtime state in private encoder structures rather than public configuration structs
rubric: correctness
severity: should_fix
---

**Rule.** Do not use public configuration structures (such as ctx->cfg or
vpx_codec_enc_cfg_t) to hold, preserve, or defer internal runtime state (such as
previously encoded frame dimensions or rate control state across calls).
Immediately apply incoming configuration updates to ctx->cfg, and maintain
internal frame-to-frame state in dedicated private encoder structures (such as
cpi / VP8_COMP / VP9_COMP).

**Why.** Public configuration structs represent caller-visible state and should
immediately reflect parameters set via configuration API calls. Altering or
deferring updates to public config fields to pass internal state causes client
applications or inspection tools querying configuration between calls to observe
stale, mutated, or incorrect settings.

## Examples

Bad:

```cpp
// In encoder_set_config:
const int last_encoded_width = ctx->cfg.g_w;
ctx->cfg = *cfg;
vp9_change_config(ctx->cpi, &ctx->oxcf);
ctx->cfg.g_w = last_encoded_width;  // Revert public config until next encode
```

Good:

```cpp
// In encoder_set_config: track dimensions in internal cpi state
if (ctx->cpi->last_coded_width &&
    !valid_ref_frame_size(ctx->cpi->last_coded_width, ctx->cpi->last_coded_height,
                          cfg->g_w, cfg->g_h)) {
  force_key = 1;
}
ctx->cfg = *cfg;
```
