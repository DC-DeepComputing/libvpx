---
name: libvpx/vpx/no-dedicated-allocators-for-codec-controls
description: When defining public codec controls in vpx/ headers, let callers manage payload buffers instead of adding dedicated allocation and free helper functions
rubric: design
severity: should_fix
---

**Rule.** Do not add dedicated memory allocation or deallocation functions to
the public API for specific codec controls or structured payloads. Document
required buffer dimensions, element counts, or size calculations in the codec
control's Doxygen comments and require callers to allocate and free the memory.

**Why.** Exposing one-off allocation and free functions for individual codec
controls clutters the public API surface, expands the shared library ABI, and
creates permanent maintenance overhead. Requiring callers to manage buffer
memory keeps the API surface lean and gives applications full control over their
memory management strategy.

## Examples

Bad:

```cpp
// In vpx/vpx_encoder.h:
vpx_codec_err_t vpx_allocate_tpl_frame_stats_list(TplFrameStats **data);
vpx_codec_err_t vpx_free_tpl_frame_stats_list(TplFrameStats *data);
```

Good:

```cpp
// In vpx/vp8cx.h:
/*!\brief Codec control function to get TPL stats.
 *
 * Caller must allocate and free a buffer of size:
 * MAX_ARF_GOP_SIZE * sizeof(TplFrameStats).
 */
VP8E_GET_TPL_STATS,
VPX_CTRL_USE_TYPE(VP8E_GET_TPL_STATS, TplFrameStats *)
```
