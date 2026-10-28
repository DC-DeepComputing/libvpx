---
name: libvpx/qualify-mode-specific-function-names
description: When defining mode-specific or condition-restricted helpers, append explicit mode qualifiers or _mode suffixes, or consolidate fragmented checks into should_run_<feature> predicates
rubric: readability
severity: should_fix
---

**Rule.** When defining or declaring functions or helpers whose logic applies
exclusively to a specific operational mode, preset, or condition (such as
simulcast, SVC, RTC, bypass, or external rate control), include the specific
qualifier in the function name (e.g., _simulcast, _rtc, _svc, _bypass_mode).
When naming helpers after modes that could be misinterpreted as action verbs or
general operations (such as VP9E_TEMPORAL_LAYERING_MODE_BYPASS), append an
explicit _mode suffix (e.g., vp9_svc_update_ref_frame_bypass_mode rather than
vp9_svc_update_ref_frame_bypass). Alternatively, consolidate fragmented
call-site condition checks and partial helpers into a comprehensive,
self-describing predicate function (e.g., should_run_<feature>). Avoid overly
generic or unadorned names that misleadingly imply universal applicability when
logic is restricted to a specific mode or edge case.

**Why.** Overly generic function names obscure configuration-specific
constraints and mislead callers into assuming universal applicability across
incompatible operating modes. Furthermore, omitting _mode on mode-specific
identifiers can cause mode names (like bypass) to read as action verbs (e.g.,
bypassing updates entirely) rather than designating a mode handler.
Consolidating fragmented condition checks into unified predicates or qualifying
restricted helpers clarifies execution preconditions and prevents incorrect
invocations.

## Examples

Bad:

```cpp
// Ambiguous name reads as an action verb rather than mode handler:
static void vp9_svc_update_ref_frame_bypass(VP9_COMP *const cpi) { ... }

// Implements reference updates specifically for simulcast key frames:
void vp9_svc_update_ref_frame_key(VP9_COMP *const cpi);

// Overly broad name for helper checking only a narrow non-ARF edge case:
static INLINE int ext_rc_tpl_pass(VP9_COMP *cpi) {
  return cpi->ext_ratectrl.ready && cm->frame_type != KEY_FRAME &&
         cpi->twopass.gf_group.update_type[1] != ARF_UPDATE;
}
if ((gf_group_index == 1 &&
     cpi->twopass.gf_group.update_type[gf_group_index] == ARF_UPDATE &&
     cpi->sf.enable_tpl_model) ||
    ext_rc_tpl_pass(cpi)) { ... }
```

Good:

```cpp
// Explicit '_mode' suffix clarifies mode-specific handler:
static void vp9_svc_update_ref_frame_bypass_mode(VP9_COMP *const cpi) { ... }

// Mode qualifier explicitly documents restricted applicability:
void vp9_svc_update_ref_frame_key_simulcast(VP9_COMP *const cpi);

// Unified predicate helper with clear name handling all feature conditions:
static INLINE int should_run_tpl(VP9_COMP *cpi, int gf_group_index) {
  if (!cpi->sf.enable_tpl_model) return 0;
  if (gf_group_index == 1 &&
      cpi->twopass.gf_group.update_type[gf_group_index] == ARF_UPDATE)
    return 1;
  return cpi->ext_ratectrl.ready && cm->frame_type != KEY_FRAME &&
         cpi->twopass.gf_group.update_type[1] != ARF_UPDATE;
}
if (should_run_tpl(cpi, gf_group_index)) { ... }
```
