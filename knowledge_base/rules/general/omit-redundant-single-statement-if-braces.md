---
name: libvpx/omit-redundant-single-statement-if-braces
description: When writing simple single-statement if conditionals, omit redundant curly braces rather than wrapping the single body statement in braces
rubric: readability
severity: nit
---

**Rule.** Do not wrap simple single-statement `if` conditions in curly braces.
Omit the braces and keep the conditional concise, placing the body on the same
line or on the next indented line.

**Why.** libvpx style favors concise formatting for simple single-statement
conditionals; unnecessary curly braces add visual clutter without improving
safety or readability.

## Examples

Bad:

```cpp
if (is_one_pass_cbr_svc(cpi)) {
  vp9_svc_update_ref_frame(cpi);
}
```

Good:

```cpp
if (is_one_pass_cbr_svc(cpi)) vp9_svc_update_ref_frame(cpi);
```
