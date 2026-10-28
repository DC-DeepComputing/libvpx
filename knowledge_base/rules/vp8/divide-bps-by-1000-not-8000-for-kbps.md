---
name: libvpx/vp8/divide-bps-by-1000-not-8000-for-kbps
description: When converting uncompressed raw stream bitrate in bits per second to kilobits per second (kbps), divide by 1000 rather than 8000
rubric: correctness
severity: should_fix
---

**Rule.** When converting an uncompressed raw stream bitrate (`width * height *
bit_depth * planes * framerate`) from bits per second (bps) to kilobits per
second (kbps) for rate control comparison or capping against
`rc_target_bitrate`, divide by 1000 rather than 8000. Do not introduce an extra
8x byte divisor when `bit_depth` is already specified in bits per sample.

**Why.** Target bitrates such as `rc_target_bitrate` are defined in kilobits per
second (kbps). Because `bit_depth` is already in bits per sample, multiplying
dimensions, bit depth, planes, and framerate produces bits per second; dividing
by 8000 incorrectly converts to kilobytes per second (kBps), resulting in a raw
bitrate value 8x smaller than intended.

## Examples

Bad:

```cpp
// Incorrect: dividing by 8000 applies an extra 8x byte divisor, producing kBps instead of kbps.
raw_target_rate = (unsigned int)(((int64_t)oxcf->width * oxcf->height *
                                  oxcf->bit_depth * 3) *
                                 oxcf->init_framerate / 8000);
cfg->rc_target_bitrate =
    VPXMIN(VPXMIN(raw_target_rate, cfg->rc_target_bitrate), 1000000);
```

Good:

```cpp
// Correct: dividing bps by 1000 produces kbps matching rc_target_bitrate.
raw_target_rate = (unsigned int)(((int64_t)oxcf->width * oxcf->height *
                                  oxcf->bit_depth * 3) *
                                 oxcf->init_framerate / 1000);
cfg->rc_target_bitrate =
    VPXMIN(VPXMIN(raw_target_rate, cfg->rc_target_bitrate), 1000000);
```
