---
name: libvpx/vp8/never-drop-key-frames
description: When implementing or updating frame-dropping logic in encoder rate control or skip modes, ensure key frames are never dropped by checking cm->frame_type != KEY_FRAME
rubric: correctness
severity: should_fix
---

**Rule.** When implementing or modifying frame-dropping logic (such as rate
control buffer drops, uncoded/empty frame skipping, or decimation), verify that
key frames are explicitly excluded by checking cm->frame_type != KEY_FRAME (or
cpi->common.frame_type != KEY_FRAME).

**Why.** Key frames serve as critical synchronization points and reference bases
for video stream decoding. Dropping a key frame disrupts stream initialization,
desynchronizes reference buffers, and breaks decoder continuity.

## Examples

Bad:

```cpp
if (cpi->common.MBs == cpi->mb.skip_true_count &&
    (cpi->drop_frame_count & 7) != 7) {
  // Drop uncoded frame without checking frame type
  cpi->drop_frame = 1;
}
```

Good:

```cpp
if (cpi->common.MBs == cpi->mb.skip_true_count &&
    (cpi->drop_frame_count & 7) != 7 && cm->frame_type != KEY_FRAME) {
  // Key frames are explicitly preserved
  cpi->drop_frame = 1;
}
```
