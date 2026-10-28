---
name: libvpx/avoid-unrelated-changes-and-scope-creep
description: When reviewing a patch, ensure all modified lines directly support the stated commit intent and exclude unrelated feature changes, debug logic, or drive-by edits
rubric: change_scope
severity: should_fix
---

**Rule.** Do not include unintended or unrelated modifications, debug code,
experimental flags, or drive-by refactoring in a patch. Ensure every change in
the CL directly supports the stated purpose described in the commit message, and
split unrelated changes into separate commits.

**Why.** Extraneous or unrelated modifications mixed into a patch increase
regression risks, complicate code review, and make bisection and cherry-picking
difficult when investigating bugs.

## Examples

Bad:

```cpp
// Commit message: "Fix minimum frame size calculation in rate control"
int rc_get_min_frame_size(const VP9_COMP *cpi) {
  return cpi->oxcf.min_frame_size;
}

// Unrelated edit mixed into the same patch:
cpi->multi_layer_arf = 1;
```

Good:

```cpp
// Commit message: "Fix minimum frame size calculation in rate control"
int rc_get_min_frame_size(const VP9_COMP *cpi) {
  return cpi->oxcf.min_frame_size;
}

// Unrelated flag or feature adjustments are omitted or moved to a separate CL.
```
