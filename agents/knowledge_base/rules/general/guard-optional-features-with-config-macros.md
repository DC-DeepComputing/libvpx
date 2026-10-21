---
name: libvpx/guard-optional-features-with-config-macros
rubric: correctness
severity: blocking
---

**Rule.** Guard declarations, feature headers, enum members, struct fields,
function definitions, and callsites for optional or experimental features with
#if CONFIG_<FEATURE> preprocessor checks (including #include "./vpx_config.h"
where needed). Do not define optional feature enum constants, prototypes, struct
fields, or implementation functions unconditionally when the feature can be
disabled at configure time.

**Why.** In libvpx, experimental or optional features are enabled via
configure-time build flags defining CONFIG_<FEATURE> macros. Unconditionally
defining feature enumerators shifts subsequent constants (such as count
terminators), while leaving definitions or declarations unguarded exposes
inactive APIs, risks undefined symbols, unused-symbol warnings, or ABI
mismatches, and causes build breakages when the feature is toggled off.

## Examples

Bad:

```cpp
// 1. Unguarded enum member shifts count terminators:
typedef enum {
  LOOKAHEAD_AQ = 7,
  SUBTITLES_AQ = 8,
  AQ_MODE_COUNT
} AQ_MODE;

// 2. Unguarded header declarations and function definitions:
// vp9/encoder/vp9_aq_subtitles.h
int vp9_subtitles_setup(struct VP9_COMP *const cpi);

// vp9/encoder/vp9_aq_subtitles.c
int vp9_subtitles_setup(VP9_COMP *const cpi) {
  return 0;
}
```

Good:

```cpp
// 1. Guarded enum member:
typedef enum {
  LOOKAHEAD_AQ = 7,
#if CONFIG_AQ_SUBTITLES
  SUBTITLES_AQ = 8,
#endif
  AQ_MODE_COUNT
} AQ_MODE;

// 2. Guarded header declarations and function definitions:
// vp9/encoder/vp9_aq_subtitles.h
#include "./vpx_config.h"
#if CONFIG_AQ_SUBTITLES
int vp9_subtitles_setup(struct VP9_COMP *const cpi);
#endif  // CONFIG_AQ_SUBTITLES

// vp9/encoder/vp9_aq_subtitles.c
#if CONFIG_AQ_SUBTITLES
int vp9_subtitles_setup(VP9_COMP *const cpi) {
  return 0;
}
#endif  // CONFIG_AQ_SUBTITLES
```
