---
name: libvpx/document-non-obvious-function-parameters
description: When declaring or defining functions with non-obvious parameters, mathematical variables, flags, or sentinel values, document their meaning and special cases
rubric: readability
severity: should_fix
---

**Rule.** When declaring or defining functions with non-obvious parameters—such
as sign multipliers (+1/-1), directional flags, mode toggles, single-letter
mathematical and matrix identifiers, or special sentinel values (such as -1,
NULL, or designated constants indicating unencoded, omitted, or uncomputed
values)—explicitly document what each parameter represents in a header
declaration or function definition comment, including domain concepts, units,
and sentinel special cases.

**Why.** Non-obvious parameter values, integer multipliers, single-letter
mathematical symbols, and sentinel values (-1, NULL) are opaque to callers and
maintainers. Without clear documentation in declaration comments, callers may
pass invalid inputs, misunderstand polarity or coordinate frames, misinterpret
default or sentinel states, and introduce subtle calculation bugs or
regressions.

## Examples

Bad:

```cpp
// Variant 1: Undocumented sign multiplier and mathematical matrix parameters
static void update_buffer_level_svc_preencode(VP9_COMP *cpi, int sgn);

static FloatMV get_smooth_motion_vector(const FloatMV search_mv,
                                        const FloatMV *tmp_mf,
                                        const int (*M)[4], int rows, int cols,
                                        int row, int col, float alpha);

// Variant 2: Undocumented sentinel parameter value (-1 for unencoded frame)
// Test if encoded frame will significantly overshoot target bitrate.
int vp9_encodedframe_overshoot(VP9_COMP *cpi, int frame_size, int *q);
```

Good:

```cpp
// Variant 1: Parameter polarity and mathematical symbols documented
// The "sgn" parameter indicates whether to fill up (+1) or leak (-1) the
// buffer. Pre-encode normally fills up, but SVC dropped frames require leaking.
static void update_buffer_level_svc_preencode(VP9_COMP *cpi, int sgn);

// M is the local variance of the reference frame, and alpha is the smoothing factor.
static FloatMV get_smooth_motion_vector(const FloatMV search_mv,
                                        const FloatMV *tmp_mf,
                                        const int (*M)[4], int rows, int cols,
                                        int row, int col, float alpha);

// Variant 2: Sentinel parameter value clearly documented
// Test if encoded frame will significantly overshoot target bitrate.
// frame_size = -1 means frame has not yet been encoded.
int vp9_encodedframe_overshoot(VP9_COMP *cpi, int frame_size, int *q);
```
