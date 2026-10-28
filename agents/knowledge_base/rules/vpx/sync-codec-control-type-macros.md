---
name: libvpx/vpx/sync-codec-control-type-macros
description: When removing or modifying codec control enum IDs in vpx/ headers, update or remove matching VPX_CTRL_USE_TYPE and VPX_CTRL_* type-binding macro definitions
rubric: correctness
severity: should_fix
---

**Rule.** When removing, renaming, or modifying a public codec control enum ID
in vpx/ headers (such as vpx/vp8cx.h, vpx/vp8dx.h, or vpx/vp8.h), ensure the
corresponding VPX_CTRL_USE_TYPE (or VPX_CTRL_USE_TYPE_DEPRECATED) macro binding
and #define VPX_CTRL_* definition are also removed or updated.

**Why.** Public codec controls in libvpx use VPX_CTRL_USE_TYPE pairings to
declare expected argument types for vpx_codec_control(). When a control ID is
deleted or renamed, leaving orphaned macro bindings creates dead code and
misleading type associations in public API headers.

## Examples

Bad:

```cpp
/* In vpx/vp8cx.h: VP9E_SET_TEMPORAL_LAYERING_MODE was removed from the enum,
 * but orphaned type associations remain at the bottom of the header. */
VPX_CTRL_USE_TYPE(VP9E_SET_TEMPORAL_LAYERING_MODE, int)
#define VPX_CTRL_VP9E_SET_TEMPORAL_LAYERING_MODE
```

Good:

```cpp
/* When removing VP9E_SET_TEMPORAL_LAYERING_MODE from the enum, also remove its
 * matching VPX_CTRL_USE_TYPE macro binding and VPX_CTRL_* definition. */
```
