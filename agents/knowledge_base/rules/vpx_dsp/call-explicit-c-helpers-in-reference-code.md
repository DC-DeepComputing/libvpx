---
name: libvpx/vpx_dsp/call-explicit-c-helpers-in-reference-code
description: In generic C reference implementations (*_c functions), call explicit *_c helper functions instead of RTCD-dispatched symbols to prevent SIMD overreads on unpadded stack buffers
rubric: correctness
severity: should_fix
---

**Rule.** In generic C reference implementations (*_c functions) and their
internal helper routines, invoke explicit *_c helper functions (such as
vpx_comp_avg_pred_c or vpx_convolve8_c) rather than RTCD-dispatched symbols
(such as vpx_comp_avg_pred or vpx_convolve8). Do not rely on dynamic RTCD
dispatch when computing intermediate sub-blocks or predictor averages from
within C baseline code.

**Why.** RTCD-dispatched function pointers resolve at runtime to SIMD-optimized
implementations (e.g., SSE2, NEON, VSX). SIMD routines often require memory
alignment or overread beyond logical block boundaries (e.g., loading 16-byte
vector registers), which causes out-of-bounds buffer overreads, ASan violations,
or memory faults when executed on unpadded stack buffers sized strictly for
scalar C reference code.

## Examples

Bad:

```cpp
// Inside generic C reference implementation
uint8_t temp2[H * W];
// ...
// BUG: Calls RTCD-dispatched symbol which resolves to SIMD kernel that overreads temp2
vpx_comp_avg_pred(temp3, second_pred, W, H, temp2, W);
```

Good:

```cpp
// Inside generic C reference implementation
uint8_t temp2[H * W];
// ...
// Correct: Calls C reference helper directly, avoiding SIMD overread requirements
vpx_comp_avg_pred_c(temp3, second_pred, W, H, temp2, W);
```
