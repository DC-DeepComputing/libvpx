---
name: libvpx/document-mode-bypass-and-error-suppression-rationale
description: When suppressing errors, assertions, or computations for specific operational modes or features, document why the bypassed data or check is safe to skip
rubric: readability
severity: nit
---

**Rule.** When suppressing an error, assertion, or computation for a specific
operational mode or feature, explain in comments why the bypassed data or check
is not required and why skipping it is safe. Do not write bare assertions that a
check or calculation is 'not critical' without describing the underlying
invariants and why downstream logic is unaffected.

**Why.** Bare assertions that an error or calculation is non-critical fail to
explain the operational invariant to future maintainers. Documenting why the
calculation is unneeded and why skipping it causes no side effects prevents
maintainers from misinterpreting bypasses as bugs or introducing regressions
during refactoring.

## Examples

Bad:

```cpp
// Since this part is not critical for ext_ratectrl, turn off the error
// reporting for now.
if (cpi->ext_ratectrl.ready) {
  break;
}
```

Good:

```cpp
// In ext_ratectrl, gop_coding_frames includes both show and no-show frames,
// so accessing first_pass_info (which only contains show frames) triggers
// an out-of-range error. This calculation only computes gf_group_err for
// internal rate control, which is unused when external rate control is active.
// Hence, skipping this error check and calculation is safe.
if (cpi->ext_ratectrl.ready) {
  break;
}
```
