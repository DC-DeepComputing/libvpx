---
name: libvpx/vp9/index-frame-buffers-in-multi-frame-loops
description: When iterating over multi-frame groups in VP9 GOP or lookahead analysis, access each frame's buffer via indexed picture arrays rather than xd->cur_buf
rubric: correctness
severity: should_fix
---

**Rule.** When iterating over multi-frame groups or pictures in GOP and
lookahead analysis (such as TPL or temporal filtering loops), access each
frame's buffer directly from the indexed picture array (such as
gf_picture[frame_idx].frame) rather than reading xd->cur_buf or cpi->Source.
Macroblock descriptor pointers like xd->cur_buf represent the currently active
frame context and do not track the iteration index across multi-frame sequences.

**Why.** In multi-frame analysis loops, xd->cur_buf is not updated per
iteration. Accessing xd->cur_buf inside the loop causes all iterations to
operate on or dump the same single active frame buffer instead of the intended
frame at each sequence index.

## Examples

Bad:

```cpp
int frame_idx;
for (frame_idx = 1; frame_idx < tpl_group_frames; ++frame_idx) {
  // Bug: xd->cur_buf points to the active frame, not the indexed GOP frame.
  dump_frame_buf(xd->cur_buf);
}
```

Good:

```cpp
int frame_idx;
for (frame_idx = 1; frame_idx < tpl_group_frames; ++frame_idx) {
  // Correct: Access the frame buffer corresponding to the current GOP index.
  dump_frame_buf(gf_picture[frame_idx].frame);
}
```
