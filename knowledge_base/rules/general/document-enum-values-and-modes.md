---
name: libvpx/document-enum-values-and-modes
description: When defining or documenting enum values, configuration modes, or macro flags, describe each value's semantics, caller expectations, conditional triggers, and prerequisites in comments
rubric: readability
severity: should_fix
---

**Rule.** When defining or extending enum types, configuration modes, or macro
flags (including public API constants and internal status/decision enums),
document each enumerator or flag value with comments or Doxygen tags (/*!\brief
... */, /**< ... */). Explain what each state or flag signifies (including
baseline or default values), expected caller behavior, and any conditional
semantics, triggers, or configuration prerequisites (such as dependent settings
or struct fields). Avoid leaving default values undocumented or phrasing
conditional behaviors as unconditional actions.

**Why.** Enum constants, decision flags, and configuration modes define
contracts across public APIs and internal subsystems. Leaving values (such as
baseline/default modes) undocumented or omitting required configuration
prerequisites and conditional triggers forces callers and maintainers to guess
operational semantics, leading to invalid configurations, integration bugs, or
unexpected runtime behavior.

## Examples

Bad:

```cpp
/*!\brief The control type of the inference API.
 * In VPX_RC_QP mode, the external rate control model determines the QP.
 */
typedef enum vpx_rc_type {
  VPX_RC_NONE = 0,  // Bad: Baseline mode is left undocumented.
  VPX_RC_QP = 1 << 0,
} vpx_rc_type_t;

// Bad: Unconditional phrasing for conditional enum behavior
typedef enum {
  CONSTRAINED_LAYER_DROP, /**< Drop this and all higher resolution layers. */
  LAYER_DROP,             /**< Any spatial layer can drop. */
} SVC_LAYER_DROP_MODE;

// Bad: Flag documentation omits required configuration prerequisite
/**< Calculate PSNR on this frame */
#define VPX_EFLAG_CALCULATE_PSNR (1 << 1)
```

Good:

```cpp
/*!\brief The control type of the inference API.
 * In VPX_RC_NONE mode, external rate control does not determine QP (baseline).
 * In VPX_RC_QP mode, external rate control model determines the QP.
 */
typedef enum vpx_rc_type {
  VPX_RC_NONE = 0,
  VPX_RC_QP = 1 << 0,
} vpx_rc_type_t;

// Good: Accurately documents conditional trigger and dependencies
typedef enum {
  CONSTRAINED_LAYER_DROP,
  /**< Upper layers are constrained to drop if current layer drops. */
  LAYER_DROP,             /**< Any spatial layer can drop. */
} SVC_LAYER_DROP_MODE;

// Good: Explicitly documents required configuration preconditions
/**< Calculate PSNR on this frame, requires g_lag_in_frames to be 0 */
#define VPX_EFLAG_CALCULATE_PSNR (1 << 1)
```
