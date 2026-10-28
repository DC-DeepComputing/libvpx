---
name: libvpx/align-predicate-helper-names-with-condition-scope
description: When defining helper functions or boolean predicates, avoid mode- or condition-specific qualifiers in function names unless the body actually verifies that condition
rubric: readability
severity: should_fix
---

**Rule.** Do not include overly specific mode, rate-control (e.g., CBR, VBR),
frame type, or feature qualifiers in helper or boolean predicate function names
if the implementation does not actually check or require that condition. Ensure
helper and predicate names accurately reflect the exact conditions verified
within the function body so callers and nested branches for other modes do not
appear contradictory or unreachable.

**Why.** When a helper or predicate function name implies a specific sub-mode
(such as CBR) but its implementation tests a broader condition (such as general
1-pass SVC), callers containing branches for alternative modes (such as VBR)
appear misleading or dead to readers and reviewers. Overly narrow qualifiers
confuse maintainers about what invariants the function actually enforces.

## Examples

Bad:

```cpp
// Misleading helper name implies CBR-only, but tests any 1-pass SVC
static INLINE int is_one_pass_cbr_svc(const struct VP9_COMP *const cpi) {
  return (cpi->use_svc && cpi->oxcf.pass == 0);
}

if (is_one_pass_cbr_svc(cpi)) {
  if (cpi->oxcf.rc_mode == VPX_CBR) {
    target = vp9_calc_pframe_target_size_one_pass_cbr(cpi);
  } else {
    // Appears unreachable or contradictory due to misleading helper name
    target = vp9_calc_pframe_target_size_one_pass_vbr(cpi);
  }
}
```

Good:

```cpp
// Accurate helper name reflecting the actual conditions evaluated
static INLINE int is_one_pass_svc(const struct VP9_COMP *const cpi) {
  return (cpi->use_svc && cpi->oxcf.pass == 0);
}

if (is_one_pass_svc(cpi)) {
  if (cpi->oxcf.rc_mode == VPX_CBR) {
    target = vp9_calc_pframe_target_size_one_pass_cbr(cpi);
  } else {
    target = vp9_calc_pframe_target_size_one_pass_vbr(cpi);
  }
}
```
