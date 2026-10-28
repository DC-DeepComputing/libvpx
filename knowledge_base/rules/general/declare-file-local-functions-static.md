---
name: libvpx/declare-file-local-functions-static
description: Declare translation-unit-local helpers static without subsystem prefixes, and prefix non-static exported functions with their subsystem identifier rather than generic or foreign names
rubric: readability
severity: should_fix
---

**Rule.** Declare functions and helper routines referenced only within a single
translation unit as static (or static INLINE / static inline) and omit library
or subsystem prefixes (e.g., vpx_, vp8_, vp9_, vpx_dsp_, vpx_scale_) from their
names. Conversely, prefix all non-static functions and exported declarations
with their established codec or subsystem namespace prefix (e.g., vpx_, vp8_,
vp9_, vpx_dsp_, vp9_denoiser_*, vp9_cyclic_refresh_*). Do not leave file-local
helpers with default external linkage, do not place action verbs before
subsystem names (e.g., use vp9_denoiser_reset_* instead of reset_denoiser_*),
and do not retain foreign prefixes (e.g., av1_ or aom_) when porting code. Keep
naming uniform across declarations, definitions, and call sites.

**Why.** In C/C++, functions have external linkage by default and share a global
symbol space across translation units and shared libraries. Declaring
file-internal helpers as static isolates them to internal linkage, avoids symbol
collisions, prevents global namespace pollution, and enables compiler
optimizations such as dead-code elimination and aggressive inlining; omitting
subsystem prefixes on static helpers avoids clutter and clarifies module
boundaries. Conversely, giving non-static exported functions consistent
subsystem prefixes prevents link-time symbol collisions across modules and
client applications, indicates subsystem ownership, eliminates foreign codebase
artifacts, and keeps public subsystem APIs searchable across headers.

## Examples

Bad:

```cpp
// Bad: File-local helper functions missing static internal linkage
FloatMV get_smooth_motion_vector(const FloatMV search_mv, const FloatMV *tmp_mf,
                                 int block_col, int block_row) {
  return search_mv;
}

// Bad: Static helpers with redundant subsystem/library prefixes
static void vp9_apply_temporal_filter_luma_16(const uint8_t *y_pre, int y_stride);
static INLINE void vpx_storeu_uint32(void *dst, uint32_t v) { memcpy(dst, &v, sizeof(v)); }

// Bad: Exported non-static functions missing subsystem prefix, verb-first, or foreign prefix
void get_smooth_motion_field(const MV *search_mf, const int (*M)[4], ...);
void reset_denoiser_on_first_frame(struct VP9_COMP *const cpi);
double av1_nb_mvs_inconsistency(const MV *mv, const int_mv *nb_mvs, int mv_num);
```

Good:

```cpp
// Good: File-local helpers declared static without subsystem prefix
static FloatMV get_smooth_motion_vector(const FloatMV search_mv,
                                        const FloatMV *tmp_mf,
                                        int block_col, int block_row) {
  return search_mv;
}
static void apply_temporal_filter_luma_16(const uint8_t *y_pre, int y_stride);
static INLINE void storeu_uint32(void *dst, uint32_t v) { memcpy(dst, &v, sizeof(v)); }

// Good: Exported non-static functions prefixed with subsystem namespace
void vp9_get_smooth_motion_field(const MV *search_mf, const int (*M)[4], ...);
void vp9_denoiser_reset_on_first_frame(struct VP9_COMP *const cpi);
double vp9_nb_mvs_inconsistency(const MV *mv, const int_mv *nb_mvs, int mv_num);
```
