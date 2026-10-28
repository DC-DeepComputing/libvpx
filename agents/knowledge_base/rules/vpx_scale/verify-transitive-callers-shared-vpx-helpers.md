---
name: libvpx/vpx_scale/verify-transitive-callers-shared-vpx-helpers
description: When modifying or removing configuration logic in shared vpx_scale or vpx_dsp helpers with legacy vp8_ prefixes, verify all transitive callers across both VP8 and VP9.
rubric: correctness
severity: should_fix
---

**Rule.** When removing or modifying configuration-specific logic (such as
CONFIG_VP9_HIGHBITDEPTH) from shared utility functions with legacy vp8_
prefixes, verify all direct and indirect callers across both VP8 and VP9 codecs.
If caller requirements diverge across codecs or configurations, introduce or
split into codec-agnostic vpx_ variants rather than dropping required handling
from shared entry points.

**Why.** Utilities in shared modules (like vpx_scale) often retain legacy vp8_
prefixes while being called by both VP8 and VP9 pipelines. Removing
high-bitdepth or configuration handling without updating callers breaks VP9
operations that indirectly rely on those functions through shared wrappers (such
as vp8_yv12_copy_frame_c).

## Examples

Bad:

```cpp
// Removing high-bitdepth handling from a shared helper without updating callers
void vp8_yv12_extend_frame_borders_c(YV12_BUFFER_CONFIG *ybf) {
  // CONFIG_VP9_HIGHBITDEPTH branch removed here, but vp8_yv12_copy_frame_c
  // still delegates to vp8_yv12_extend_frame_borders_c for high-bitdepth buffers
  extend_plane(...);
}
```

Good:

```cpp
// Separate codec-agnostic / VP9 high-bitdepth helper from VP8-only helper
void vp8_yv12_extend_frame_borders_c(YV12_BUFFER_CONFIG *ybf) {
  extend_plane(...);
}

void vpx_extend_frame_borders_c(YV12_BUFFER_CONFIG *ybf) {
#if CONFIG_VP9_HIGHBITDEPTH
  if (ybf->flags & YV12_FLAG_HIGHBITDEPTH) {
    extend_plane_high(...);
    return;
  }
#endif
  extend_plane(...);
}
```
