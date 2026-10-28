---
name: libvpx/remove-unused-function-declarations
description: When modifying headers or localizing definitions, remove unused function prototypes and extern variable or table declarations to avoid orphaned header declarations
rubric: readability
severity: nit
---

**Rule.** Do not leave unused, obsolete, or orphaned function prototypes or
extern variable, array, and table declarations in header files. When deleting,
renaming, or refactoring functions, or when giving definitions static internal
linkage and localizing them to a single translation unit, remove their
corresponding header declarations immediately.

**Why.** Leaving unused function prototypes and extern declarations in header
files clutters header interfaces, creates misleading linkage expectations, and
misleads developers into believing non-existent or internal-only symbols and
interfaces are implemented and supported.

## Examples

Bad:

```cpp
// vp9_svc_layercontext.h - prototype declared but function is never defined or used
int vp9_svc_check_setting_intra_only(struct VP9_COMP *const cpi);

// vpx_dsp/loongarch/vpx_convolve_lsx.h - extern declaration left after table was made static
extern const uint8_t mc_filt_mask_arr[16 * 3];

// vpx_dsp/loongarch/vpx_convolve8_horiz_lsx.c:
static const uint8_t mc_filt_mask_arr[16 * 3] = { ... };
```

Good:

```cpp
// vp9_svc_layercontext.h - only active, implemented function prototypes are retained
void vp9_update_layer_context_change_config(struct VP9_COMP *const cpi,
                                            const int target_bandwidth);

// vpx_dsp/loongarch/vpx_convolve_lsx.h:
// (Unused extern declaration removed)

// vpx_dsp/loongarch/vpx_convolve8_horiz_lsx.c:
static const uint8_t mc_filt_mask_arr[16 * 3] = { ... };
```
