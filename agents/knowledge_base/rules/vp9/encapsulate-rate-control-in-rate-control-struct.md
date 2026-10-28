---
name: libvpx/vp9/encapsulate-rate-control-in-rate-control-struct
description: When adding rate control parameters, flags, or state variables in the VP9 encoder, place them in the RATE_CONTROL struct rather than top-level VP9_COMP
rubric: design
severity: should_fix
---

**Rule.** When adding or modifying rate-control, frame-dropping, or buffer-level
state variables and flags in the VP9 encoder, place them inside the dedicated
RATE_CONTROL struct (cpi->rc in vp9_ratectrl.h) rather than directly in the
top-level compressor struct VP9_COMP. Access and update these fields through
cpi->rc or by passing a pointer to RATE_CONTROL.

**Why.** VP9_COMP is the top-level encoder context and embeds modular subsystem
structs. Placing subsystem-specific flags directly into VP9_COMP pollutes the
compressor context, violates modular boundaries between the encoder core and the
rate control module, and hinders reusing or testing rate control logic
independently.

## Examples

Bad:

```cpp
// In vp9/encoder/vp9_encoder.h:
typedef struct VP9_COMP {
  ...
  int last_post_encode_dropped_scene_change;
  int use_post_encode_drop;
} VP9_COMP;
```

Good:

```cpp
// In vp9/encoder/vp9_ratectrl.h:
typedef struct {
  ...
  int last_post_encode_dropped_scene_change;
  int use_post_encode_drop;
} RATE_CONTROL;
```
