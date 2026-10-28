---
name: libvpx/vp8/name-decoder-context-parameters-pbi
description: When declaring or defining decoder context parameters (VP8D_COMP* or VP9Decoder*), name them 'pbi' rather than encoder-associated 'cpi' or generic identifiers
rubric: readability
severity: should_fix
---

**Rule.** When declaring or defining functions that accept a decoder context
pointer (`struct VP8D_COMP *` in VP8 or `VP9Decoder *` in VP9), name the
parameter `pbi` (playback info). Do not use encoder-associated names like `cpi`
(compression info) or generic parameter names like `comp` or single letters.

**Why.** `cpi` designates compression info (`VP8_COMP` / `VP9_COMP`), whereas
`pbi` designates playback/decompression info for decoder contexts. Using
consistent context parameter names prevents confusion between encoder and
decoder interfaces and maintains codebase uniformity across VP8 and VP9.

## Examples

Bad:

```cpp
int vp8dx_get_quantizer(const struct VP8D_COMP *cpi);
int vp9_get_raw_frame(struct VP9Decoder *comp, YV12_BUFFER_CONFIG *sd,
                      vpx_codec_iter_t *iter);
```

Good:

```cpp
int vp8dx_get_quantizer(const struct VP8D_COMP *pbi);
int vp9_get_raw_frame(struct VP9Decoder *pbi, YV12_BUFFER_CONFIG *sd,
                      vpx_codec_iter_t *iter);
```
