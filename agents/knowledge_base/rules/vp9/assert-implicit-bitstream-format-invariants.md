---
name: libvpx/vp9/assert-implicit-bitstream-format-invariants
description: When serializing bitstream headers with implicit syntax for specific profiles or color spaces, assert all implied format and subsampling invariants
rubric: correctness
severity: should_fix
---

**Rule.** When serializing bitstream headers where the codec specification omits
syntax fields under specific profile or color format combinations (such as sRGB
implicitly requiring 4:4:4 chroma sampling), explicitly assert all implicit
profile and subsampling invariants (e.g., `cm->profile == PROFILE_1 ||
cm->profile == PROFILE_3` and `cm->subsampling_x == 0 && cm->subsampling_y ==
0`) before writing implicit or unused syntax bits. Do not emit implicit header
bits without validating that internal encoder state matches the codec
specification.

**Why.** Bitstream writers rely on encoder state satisfying codec specification
constraints when syntax elements are omitted or implied. Asserting these
preconditions catches encoder misconfigurations, invalid color space mappings,
or inconsistent state transitions before generating malformed bitstreams.

## Examples

Bad:

```cpp
if (cm->color_space == VPX_CS_SRGB) {
  assert(cm->profile == PROFILE_1 || cm->profile == PROFILE_3);
  vpx_wb_write_bit(wb, 0);  // unused
}
```

Good:

```cpp
if (cm->color_space == VPX_CS_SRGB) {
  assert(cm->profile == PROFILE_1 || cm->profile == PROFILE_3);
  assert(cm->subsampling_x == 0 && cm->subsampling_y == 0);
  vpx_wb_write_bit(wb, 0);  // unused
}
```
