---
name: libvpx/document-container-timebase-and-timestamp-scaling
description: When setting container header or encoder time bases with a fixed tick rate, document the relationship between time_base and per-frame timestamp calculations.
rubric: readability
severity: nit
---

**Rule.** When configuring container file headers (such as IVF or WebM) or
encoder time bases (time_base or g_timebase) using a fixed tick rate (such as 1
/ TICKS_PER_SEC) rather than 1 / frame_rate, add a comment documenting the
timestamp scaling relationship. Explicitly state the formula showing how
per-frame timestamps multiplied by time_base produce presentation timestamps in
seconds.

**Why.** Container formats like IVF support arbitrary rational time bases, and
using a fixed tick rate rather than the reciprocal of the frame rate requires
scaling per-frame timestamps. Documenting the scaling formula prevents confusion
over why a fixed tick rate was selected and ensures timestamps remain consistent
across encoder and container muxing layers.

## Examples

Bad:

```cpp
const char *fourcc = "VP90";
vpx_rational_t time_base = make_vpx_rational(1, TICKS_PER_SEC);
ivf_write_file_header_with_video_info(out_file_, *(const uint32_t *)fourcc,
                                      num_frames_, frame_width_,
                                      frame_height_, time_base);
```

Good:

```cpp
const char *fourcc = "VP90";
// In SimpleEncode, we use time_base = 1 / TICKS_PER_SEC. The ivf_timestamp for each
// image is set to show_idx * TICKS_PER_SEC / frame_rate so that each image's
// actual timestamp in seconds is ivf_timestamp * time_base == show_idx / frame_rate.
vpx_rational_t time_base = make_vpx_rational(1, TICKS_PER_SEC);
ivf_write_file_header_with_video_info(out_file_, *(const uint32_t *)fourcc,
                                      num_frames_, frame_width_,
                                      frame_height_, time_base);
```
