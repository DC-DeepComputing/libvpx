---
name: libvpx/vp9/order-conditional-enum-modes-before-internal-modes
description: When adding conditionally compiled user-configurable modes to an enum with internal modes before a count sentinel, place them before internal modes and use auto-incrementation.
rubric: correctness
severity: should_fix
---

**Rule.** When adding conditionally compiled or optional user-configurable modes
to an enum that ends with internal-only modes before a count sentinel (_COUNT),
place the new modes before all internal modes and rely on auto-incrementing
enumerators rather than hardcoding numeric values. Do not append user modes
after internal modes or introduce branching #if preprocessor checks in
RANGE_CHECK validation.

**Why.** Appending user-configurable modes after internal modes or hardcoding
enum values breaks contiguous numbering assumptions, causing interface
RANGE_CHECK validation (e.g. 0 to _COUNT - 2) to either expose internal-only
modes to users or require fragile #if guards across build configurations.
Auto-incrementing user modes before internal sentinels ensures validation bounds
remain consistent across all build options.

## Examples

Bad:

```cpp
// In vp9_encoder.h:
typedef enum {
  NO_AQ = 0,
  ...
  LOOKAHEAD_AQ = 7,  // Internal mode
#if CONFIG_ROI_FILE_AQ
  ROI_FILE_AQ = 8,   // Placed after internal mode with hardcoded value
#endif
  AQ_MODE_COUNT
} AQ_MODE;

// In vp9_cx_iface.c:
#if !CONFIG_ROI_FILE_AQ
  RANGE_CHECK(extra_cfg, aq_mode, 0, AQ_MODE_COUNT - 2);
#else
  RANGE_CHECK(extra_cfg, aq_mode, 0, AQ_MODE_COUNT - 1);  // Inadvertently permits internal LOOKAHEAD_AQ!
#endif
```

Good:

```cpp
// In vp9_encoder.h:
typedef enum {
  NO_AQ = 0,
  VARIANCE_AQ = 1,
  ...
#if CONFIG_ROI_FILE_AQ
  ROI_FILE_AQ,  // Placed before internal modes with auto-increment
#endif
  LOOKAHEAD_AQ,  // Internal mode remains immediately before _COUNT
  AQ_MODE_COUNT
} AQ_MODE;

// In vp9_cx_iface.c:
// Clean, unconditional range check encompasses all user modes across build configs
RANGE_CHECK(extra_cfg, aq_mode, 0, AQ_MODE_COUNT - 2);
```
