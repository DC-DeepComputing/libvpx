---
name: libvpx/vp8/keep-encoder-decoder-specific-types-out-of-common
description: When defining structures, constants, logic, or headers in common/, scope encoder-only or decoder-only features within encoder/ or decoder/ to maintain subsystem layering
rubric: design
severity: should_fix
---

**Rule.** Do not place data structures, constants, header files, or logic in
common/ unless they are actively required by both the encoder and decoder
pipelines. Scope encoder-only features (such as adaptive quantization, rate
control, motion estimation, level calculation, or RDO) and decoder-only routines
within encoder/ or decoder/ directories respectively.

**Why.** Code in common/ is compiled into both encoder and decoder builds.
Placing encoder-only or decoder-only data structures, headers, and internal
routines in common/ pollutes shared interfaces, breaks subsystem layering,
increases coupling, and introduces unnecessary definitions or dead-code binary
bloat into standalone decoder-only or encoder-only configurations.

## Examples

Bad:

```cpp
// In vp9/common/vp9_common_data.h (encoder-only level stats in common/)
typedef struct {
  VP9_LEVEL_STATS level_stats;
  VP9_LEVEL_SPEC level_spec;
} VP9_LEVEL_INFO;

void vp9_calculate_level_stats(VP9_LEVEL_INFO *level_info);

// In vp9/common/vp9_alt_ref_aq_common.h (encoder-only AQ logic in common/)
typedef enum {
  ALTREF_AQ_BEFORE,
  ALTREF_AQ_AT,
  ALTREF_AQ_AFTER,
  ALTREF_AQ_NSTATES
} ALT_REF_SEG_SWAP_STATE;

struct ALT_REF_AQ *vp9_alt_ref_aq_create(void);
```

Good:

```cpp
// In vp9/encoder/vp9_level.h (scoped to encoder/)
typedef struct {
  Vp9LevelStats level_stats;
  Vp9LevelSpec level_spec;
} Vp9LevelInfo;

void vp9_calculate_level_stats(Vp9LevelInfo *level_info);

// In vp9/encoder/vp9_alt_ref_aq.h (scoped to encoder/)
typedef enum {
  ALTREF_AQ_BEFORE,
  ALTREF_AQ_AT,
  ALTREF_AQ_AFTER,
  ALTREF_AQ_NSTATES
} ALT_REF_SEG_SWAP_STATE;

struct ALT_REF_AQ *vp9_alt_ref_aq_create(void);
```
