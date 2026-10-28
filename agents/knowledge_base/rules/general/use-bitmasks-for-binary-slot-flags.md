---
name: libvpx/use-bitmasks-for-binary-slot-flags
description: When representing sets of small binary flags or slot selections across layers or frames, use integer bitmasks instead of multi-dimensional arrays.
rubric: readability
severity: should_fix
---

**Rule.** Represent sets of small binary flags, frame reference buffers, or slot
selections across layers as integer bitmasks rather than multi-dimensional
arrays of integers or booleans. Use 1D arrays of bitmask integers (such as `int
update_buffer_slot[VPX_SS_MAX_LAYERS]`) where each bit position corresponds to a
buffer slot or flag index, manipulating active slots using bitwise operations
(`1 << slot_idx`).

**Why.** Using bitmasks produces compact struct layouts, simplifies testing,
setting, or clearing multiple slot selections in a single bitwise operation, and
aligns with libvpx API conventions for reference frame and buffer management.

## Examples

Bad:

```cpp
struct vpx_svc_ref_frame_config {
  int update_buffer_slot[8][VPX_SS_MAX_LAYERS]; /**< Update reference frames. */
};
```

Good:

```cpp
struct vpx_svc_ref_frame_config {
  int update_buffer_slot[VPX_SS_MAX_LAYERS]; /**< Update reference frames. */
};

// Usage: set buffer slots 0, 3, and 4 for spatial layer sl
ref_frame_config.update_buffer_slot[sl] = (1 << 0) | (1 << 3) | (1 << 4);
```
