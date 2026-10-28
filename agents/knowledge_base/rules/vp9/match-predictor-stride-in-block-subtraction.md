---
name: libvpx/vp9/match-predictor-stride-in-block-subtraction
description: When calling block subtraction helpers like vpx_subtract_block or vpx_highbd_subtract_block, ensure dimension and predictor stride arguments match the predictor buffer layout
rubric: correctness
severity: should_fix
---

**Rule.** When invoking block subtraction functions (such as vpx_subtract_block
and vpx_highbd_subtract_block), verify that the rows, columns, and stride
arguments (including diff_stride and pred_stride) match the actual block
dimensions (bh, bw) and buffer layout used when generating the predictor. Do not
pass hardcoded stride or dimension constants unless the buffer layout is
strictly fixed to that size.

**Why.** Hardcoding a fixed dimension or stride (such as 16 when the predictor
was generated with a width/stride of 32 or bw) causes subsequent rows of the
predictor buffer to be read from incorrect offsets, corrupting residual
calculations and rate-distortion decisions.

## Examples

Bad:

```cpp
// Hardcoding predictor stride to 16 when block width and predictor stride are 32 or dynamic:
vpx_highbd_subtract_block(
    32, 32, src_diff, 32, this_frame->y_buffer + mb_y_offset,
    this_frame->y_stride, &predictor[0], 16, xd->bd);
```

Good:

```cpp
// Match block dimensions and predictor stride to actual block width and buffer layout:
vpx_highbd_subtract_block(
    bh, bw, src_diff, bw, this_frame->y_buffer + mb_y_offset,
    this_frame->y_stride, &predictor[0], bw, xd->bd);
```
