---
name: libvpx/avoid-pseudo-condition-comments-on-else
description: When writing fallback else blocks, do not annotate them with pseudo-conditional comments; use explicit else-if checks, assert invariants, or omit the comment
rubric: readability
severity: should_fix
---

**Rule.** Do not write pseudo-conditional comments on else statements (such as
'} else { /* if (cond) */'). Omit the comment when the block serves as a general
fallback, or use an explicit else if (cond) check or debug assertion
(assert(cond)) when code relies on an assumed invariant.

**Why.** Commented-out condition checks on else statements are misleading
because the block executes for all remaining states rather than enforcing the
commented condition. If the branch genuinely requires a specific condition or
invariant, making it explicit in code or assertions prevents silent execution on
unintended inputs.

## Examples

Bad:

```cpp
if (copy_mode == 1) {
  copy_last_to_golden(cpi);
} else { /* if (copy_mode == 2) */
  copy_altref_to_golden(cpi);
}
```

Good:

```cpp
if (copy_mode == 1) {
  copy_last_to_golden(cpi);
} else if (copy_mode == 2) {
  copy_altref_to_golden(cpi);
}
```
