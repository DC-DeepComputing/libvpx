---
name: libvpx/omit-const-on-pass-by-value-scalar-parameters
description: When declaring or defining function parameters passed by value, omit const qualification on primitive and scalar types like int, bool, or size_t
rubric: readability
severity: nit
---

**Rule.** Do not qualify pass-by-value primitive or scalar function parameters
(such as int, bool, size_t, float, or uint32_t) with const in function
declarations, prototypes, or definitions. Reserve const on parameters for
pointers-to-const (e.g., const T *) or references-to-const (const T &) where
immutability of caller data is enforced.

**Why.** Primitive and scalar arguments are passed by value and copied into
local storage, so top-level const qualification on value parameters has no
effect on callers and adds unnecessary visual clutter to function interfaces and
signatures.

## Examples

Bad:

```cpp
static void trim_tpl_stats(struct vpx_internal_error_info *error_info,
                           VpxTplGopStats *tpl_gop_stats,
                           const int extra_frames);

int compute_frame_cost(const VP9_COMP *cpi, const int speed, const bool is_key);
```

Good:

```cpp
static void trim_tpl_stats(struct vpx_internal_error_info *error_info,
                           VpxTplGopStats *tpl_gop_stats,
                           int extra_frames);

int compute_frame_cost(const VP9_COMP *cpi, int speed, bool is_key);
```
