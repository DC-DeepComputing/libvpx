---
name: libvpx/extract-self-contained-pipeline-logic-into-helpers
description: When implementing multi-step feature logic inside large codec pipeline functions, extract self-contained processing steps into dedicated static helper functions
rubric: design
severity: should_fix
---

**Rule.** Extract complex or self-contained processing steps (such as
post-encode drop handling, feature-specific rate control updates, or mode
filtering) from large pipeline and driver functions into dedicated static helper
functions. Keep top-level driver routines focused on coordinating high-level
pipeline stages.

**Why.** Inlining multi-step feature logic into large encoding or decoding
pipeline functions increases function complexity, obscures high-level control
flow, and impairs readability and maintainability.

## Examples

Bad:

```cpp
static void encode_frame_to_data_rate(VP9_COMP *cpi, size_t *size, ...) {
  ...
  vp9_pack_bitstream(cpi, dest, size);

  // Inlined postencode drop logic and buffer updates
  if (cpi->rc.use_post_encode_drop && cm->base_qindex < cpi->rc.worst_quality) {
    size_t frame_size = *size << 3;
    int64_t new_buffer_level =
        cpi->rc.buffer_level + cpi->rc.avg_frame_bandwidth - frame_size;
    if (new_buffer_level < 0) {
      *size = 0;
      vp9_rc_postencode_update_drop_frame(cpi);
      ...
      return;
    }
  }
  ...
}
```

Good:

```cpp
static int post_encode_drop_screen_content(VP9_COMP *cpi, size_t *size) {
  // Factored helper encapsulates postencode drop logic and buffer updates
  ...
  return is_dropped;
}

static void encode_frame_to_data_rate(VP9_COMP *cpi, size_t *size, ...) {
  ...
  vp9_pack_bitstream(cpi, dest, size);

  // Postencode drop for CBR screen-content mode.
  if (post_encode_drop_screen_content(cpi, size)) return;
  ...
}
```
