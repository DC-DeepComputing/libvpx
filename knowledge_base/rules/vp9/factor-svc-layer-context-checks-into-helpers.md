---
name: libvpx/vp9/factor-svc-layer-context-checks-into-helpers
description: When querying SVC layer context state or compound layer properties across encoder modules, extract shared predicate helpers in vp9_svc_layercontext rather than repeating lookups
rubric: design
severity: should_fix
---

**Rule.** Extract repeated SVC layer context lookups and multi-step condition
checks (such as computing layer indices with LAYER_IDS_TO_IDX and testing
LAYER_CONTEXT properties like is_key_frame) into centralized helper functions in
vp9_svc_layercontext.c (declared in vp9_svc_layercontext.h). Avoid open-coding
duplicate layer indexing and context dereferences across multiple encoder
modules.

**Why.** Duplicating multi-step layer context lookups and condition evaluations
across different encoder modules clutters caller code, increases maintenance
burden, and risks subtle behavioral drift when layer indexing or context
invariants change.

## Examples

Bad:

```cpp
// In vp9/encoder/vp9_pickmode.c or vp9/encoder/vp9_denoiser.c
int layer = LAYER_IDS_TO_IDX(cpi->svc.spatial_layer_id,
                             cpi->svc.temporal_layer_id,
                             cpi->svc.number_temporal_layers);
LAYER_CONTEXT *lc = &cpi->svc.layer_context[layer];
if (denoise_svc(cpi) && !lc->is_key_frame) {
  // Execute denoising for non-key SVC frame
}
```

Good:

```cpp
// In vp9/encoder/vp9_svc_layercontext.c
int vp9_denoise_svc_non_key(VP9_COMP *const cpi) {
  int layer = LAYER_IDS_TO_IDX(cpi->svc.spatial_layer_id,
                               cpi->svc.temporal_layer_id,
                               cpi->svc.number_temporal_layers);
  LAYER_CONTEXT *lc = &cpi->svc.layer_context[layer];
  return denoise_svc(cpi) && !lc->is_key_frame;
}

// In callers across vp9/encoder/
if (vp9_denoise_svc_non_key(cpi)) {
  // Execute denoising for non-key SVC frame
}
```
