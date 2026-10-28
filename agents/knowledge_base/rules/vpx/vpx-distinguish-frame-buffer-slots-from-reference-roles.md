---
name: libvpx/vpx/vpx-distinguish-frame-buffer-slots-from-reference-roles
description: When declaring fields, variables, or arrays indexing frame buffer pool slots, use explicit names like buffer_slot or fb_idx rather than reference
rubric: readability
severity: should_fix
---

**Rule.** When naming struct members, function arguments, or variables that
represent or index physical frame buffer pool slots (indices 0 to REF_FRAMES -
1), use explicit terminology such as buffer_slot or fb_idx. Reserve terms like
ref_frame, reference, and specific role acronyms (LAST, GOLDEN, ALTREF) for
logical reference frame roles and prediction flags.

**Why.** In VP8 and VP9, logical reference roles point to physical frame buffer
slots in the buffer pool. Naming physical slot indices or slot update masks
using generic 'reference' terms creates ambiguity with logical reference frame
roles (such as reference_last or update_golden) and obscures whether a value
represents a physical buffer pool index or a logical reference role.

## Examples

Bad:

```cpp
// Ambiguous 'reference' identifier for physical frame buffer pool slots
struct vpx_svc_ref_frame_config {
  int lst_fb_idx[VPX_SS_MAX_LAYERS];
  int update_reference[VPX_SS_MAX_LAYERS];  // Bitmask of buffer slots to update
  int reference_last[VPX_SS_MAX_LAYERS];    // Flag for LAST reference role
};
```

Good:

```cpp
// Explicit 'buffer_slot' identifier distinguishing physical slots from roles
struct vpx_svc_ref_frame_config {
  int lst_fb_idx[VPX_SS_MAX_LAYERS];
  int update_buffer_slot[VPX_SS_MAX_LAYERS];  // Explicitly names buffer slots
  int reference_last[VPX_SS_MAX_LAYERS];      // Flag for LAST reference role
};
```
