---
name: libvpx/vpx_dsp/external-linkage-for-rtcd-dsp-functions
description: When implementing RTCD architecture-specific DSP functions mirroring reference prototypes, give them external linkage without static; declare file-local helpers static without vpx_ prefix
rubric: readability
severity: should_fix
---

**Rule.** Do not declare architecture-specific DSP implementations that mirror a
global RTCD or reference function (such as vpx_<func>_<arch> mirroring
vpx_<func>_c) with static internal linkage; give them external linkage and
adhere to the reference prototype. Conversely, declare internal translation-unit
helper functions as static (or static INLINE), omit the public vpx_ prefix, and
do not retain unused parameters.

**Why.** RTCD (Runtime CPU Detection) dispatch requires architecture-specific
DSP functions to be exported with external linkage so they can be registered in
dispatch tables and invoked across translation units. Marking
reference-mirroring implementations static prevents external symbol resolution,
while giving internal helpers vpx_ prefixes pollutes global symbol namespaces.

## Examples

Bad:

```cpp
// Marking an RTCD-dispatched DSP implementation static prevents external linkage:
static void vpx_convolve_avg_mmi(const uint8_t *src, ptrdiff_t src_stride,
                                 uint8_t *dst, ptrdiff_t dst_stride,
                                 const InterpKernel *filter, int x0_q4,
                                 int x_step_q4, int y0_q4, int y_step_q4, int w,
                                 int h) {
  // ...
}
```

Good:

```cpp
// Export RTCD-dispatched DSP functions with external linkage matching reference prototype:
void vpx_convolve_avg_mmi(const uint8_t *src, ptrdiff_t src_stride,
                          uint8_t *dst, ptrdiff_t dst_stride,
                          const InterpKernel *filter, int x0_q4,
                          int x_step_q4, int y0_q4, int y_step_q4, int w,
                          int h) {
  // ...
}

// Internal helper functions remain static without the vpx_ prefix:
static INLINE void convolve_avg_mmi_helper(const uint8_t *src, uint8_t *dst,
                                          int w, int h) {
  // ...
}
```
