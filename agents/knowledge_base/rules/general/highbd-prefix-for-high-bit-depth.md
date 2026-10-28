---
name: libvpx/highbd-prefix-for-high-bit-depth
description: When naming functions, callbacks, or identifiers operating on high bit-depth video data, use the standard highbd_ prefix rather than high_
rubric: readability
severity: should_fix
---

**Rule.** Use the prefix highbd_ (e.g., highbd_predict,
vp9_highbd_build_inter_predictor, vpx_highbd_*) rather than high_ when naming
functions, callbacks, function pointers, and identifiers that operate on high
bit-depth video data.

**Why.** Libvpx uniformly uses highbd_ across DSP routines, codec modules, and
tests to denote high bit-depth processing. Using non-standard abbreviations like
high_ creates naming inconsistency and hinders searchability across the
codebase.

## Examples

Bad:

```cpp
static INLINE void high_inter_predictor(const uint8_t *src, int src_stride,
                                        uint8_t *dst, int dst_stride,
                                        const int16_t *src_y, int y_step,
                                        int w, int h, int bd);
```

Good:

```cpp
static INLINE void highbd_inter_predictor(const uint8_t *src, int src_stride,
                                          uint8_t *dst, int dst_stride,
                                          const int16_t *src_y, int y_step,
                                          int w, int h, int bd);
```
