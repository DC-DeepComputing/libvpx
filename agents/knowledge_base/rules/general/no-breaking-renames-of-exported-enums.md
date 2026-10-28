---
name: libvpx/no-breaking-renames-of-exported-enums
description: When evolving exported enum constants or API identifiers in headers, avoid unilaterally renaming in-use symbols; preserve existing names with updated comments or stage coordinated downstream updates.
rubric: correctness
severity: should_fix
---

**Rule.** When evolving or refining the semantics of exported enum constants,
control values, or API identifiers in header files, do not unilaterally rename
or delete in-use identifiers that would break downstream consumers (such as
WebRTC or Chromium). Instead, retain the existing identifier and update its doc
comments to describe the updated behavior, or introduce new identifiers
alongside backward-compatible aliases and coordinate downstream migrations
across separate CLs before removing legacy names.

**Why.** libvpx headers and codec control enums are directly referenced by
external downstream consumers such as WebRTC and Chromium. Renaming an active
identifier immediately causes compile breaks in downstream builds before caller
code can be migrated.

## Examples

Bad:

```cpp
// In vp9_svc_layercontext.h:
typedef enum {
  INTER_LAYER_PRED_ON,
  INTER_LAYER_PRED_OFF,
- INTER_LAYER_PRED_OFF_NONKEY,
+ INTER_LAYER_PRED_OFF_NONKEY_NONSYNC,
  INTER_LAYER_PRED_ON_CONSTRAINED
} INTER_LAYER_PRED;
```

Good:

```cpp
// In vp9_svc_layercontext.h:
typedef enum {
  INTER_LAYER_PRED_ON,
  INTER_LAYER_PRED_OFF,
  // Inter-layer prediction is off on non-key frames and non-sync frames.
  // Retaining original identifier for downstream (e.g. WebRTC) compatibility.
  INTER_LAYER_PRED_OFF_NONKEY,
  INTER_LAYER_PRED_ON_CONSTRAINED
} INTER_LAYER_PRED;
```
