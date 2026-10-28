---
name: libvpx/vp8/place-encoder-init-in-compressor-creation
description: When initializing general encoder state or dimensions, place setup in compressor creation (*_create_compressor) or reconfiguration routines rather than inside specific codec control handlers (ctrl_set_*).
rubric: design
severity: should_fix
---

**Rule.** Do not perform general encoder state initialization or dimension
tracking inside feature-specific codec control handlers (such as ctrl_set_*).
Place general encoder setup, default dimensions, and buffer state initialization
in compressor creation functions (such as vp9_create_compressor or
vp8_create_compressor) or top-level reconfiguration routines (*_change_config).

**Why.** Codec control handlers are optional and can be invoked in arbitrary
order or omitted entirely by callers. Coupling fundamental encoder lifecycle
state initialization into a specific control handler risks leaving state
uninitialized or corrupting state across dynamic reconfigurations.

## Examples

Bad:

```cpp
static vpx_codec_err_t ctrl_set_svc_parameters(vpx_codec_alg_priv_t *ctx,
                                               va_list args) {
  // ... handle svc parameters ...
  if (cpi->common.current_video_frame == 0) {
    cpi->initial_width = cpi->oxcf.width;
    cpi->initial_height = cpi->oxcf.height;
  }
  return VPX_CODEC_OK;
}
```

Good:

```cpp
VP9_COMP *vp9_create_compressor(const VP9EncoderConfig *oxcf,
                                BufferPool *const pool) {
  // ...
  init_frame_indexes(cm);
  cpi->initial_width = cpi->oxcf.width;
  cpi->initial_height = cpi->oxcf.height;
  cpi->tile_data = NULL;
  // ...
  return cpi;
}
```
