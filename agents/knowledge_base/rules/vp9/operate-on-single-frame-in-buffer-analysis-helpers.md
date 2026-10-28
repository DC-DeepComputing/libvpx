---
name: libvpx/vp9/operate-on-single-frame-in-buffer-analysis-helpers
description: When implementing frame buffer analysis or motion estimation helpers, accept a single frame buffer instead of iterating over fixed-size frame arrays
rubric: design
severity: should_fix
---

**Rule.** Design frame analysis, motion estimation, and feature calculation
helpers to accept and operate on a single frame buffer (`const
YV12_BUFFER_CONFIG *`) rather than accepting fixed-size arrays of frame buffers
(such as `frame[3]`) and iterating internally. Keep frame iteration loops at
caller sites in the encoding pipeline so that lower-level analysis routines
remain modular, reusable, and free of hardcoded reference frame counts.

**Why.** Operating on a single frame buffer decouples low-level analysis
calculations from encoder reference frame management. Hardcoding fixed frame
arrays inside computation helpers prevents reuse for single frames, complicates
unit testing, and embeds assumptions about reference frame counts that break
when pipeline configurations change.

## Examples

Bad:

```cpp
// Anti-pattern: Hardcoding a 3-frame array and looping internally
void get_local_structure(VP9_COMP *cpi, MACROBLOCKD *xd,
                         YV12_BUFFER_CONFIG *frame[3], BLOCK_SIZE bsize,
                         int *M[3]) {
  int i;
  for (i = 0; i < 3; ++i) {
    calculate_structure(frame[i], bsize, M[i]);
  }
}
```

Good:

```cpp
// Recommended: Operate on a single frame buffer; caller controls iteration
void vp9_get_local_structure(const YV12_BUFFER_CONFIG *ref_frame,
                             const vp9_variance_fn_ptr_t *fn_ptr, int mi_rows,
                             int mi_cols, BLOCK_SIZE bsize, int (*M)[4]) {
  calculate_structure(ref_frame, fn_ptr, mi_rows, mi_cols, bsize, M);
}

// Caller iterates over active reference frames:
for (ref_frame = LAST_FRAME; ref_frame <= ALTREF_FRAME; ++ref_frame) {
  const YV12_BUFFER_CONFIG *buf = get_ref_frame_buffer(cpi, ref_frame);
  vp9_get_local_structure(buf, fn_ptr, mi_rows, mi_cols, bsize, M[ref_frame]);
}
```
