---
name: libvpx/commit-message-accurately-reflects-logic-changes
description: When writing commit messages, ensure the subject and description accurately reflect logic and semantic changes rather than describing non-trivial refactorings as mere cleanups
rubric: readability
severity: should_fix
---

**Rule.** Ensure the commit message subject and description accurately describe
all logic, control flow, and behavioral changes introduced by the patch. Do not
characterize a change as a simple refactoring, cleanup, or redundant check
removal if statement reordering or condition updates alter program semantics,
error handling, or bounds checking.

**Why.** Inaccurate commit messages mislead reviewers and future maintainers
when investigating regressions, bisecting bugs, or auditing security-sensitive
code. Accurately explaining semantic changes ensures that non-trivial
alterations to control flow or bounds validation receive proper review scrutiny.

## Examples

Bad:

```cpp
Commit subject: vp8_decode_frame: remove unnecessary check

Commit body:
Clean up redundant bounds checking in frame parsing.
(When pointer increments were moved and bounds checking semantics were altered)
```

Good:

```cpp
Commit subject: vp8_decode_frame: improve header / partition 0 check

Commit body:
Move first_partition_length_in_bytes validation after keyframe data
parsing and merge keyframe size checks to validate buffer bounds earlier.
```
