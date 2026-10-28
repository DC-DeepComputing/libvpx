---
name: libvpx/format-inline-argument-comments
description: When writing inline parameter name comments at function call sites, format them as /*param_name=*/value without spaces and with an equals sign.
rubric: readability
severity: nit
---

**Rule.** When clarifying function arguments with inline parameter name comments
at call sites, format the comment exactly as /*param_name=*/value with no
internal spaces and the equals sign immediately before */. Do not omit the
equals sign or insert spaces around the comment and argument value.

**Why.** The exact /*param_name=*/value syntax enables static analysis tools
(such as clang-tidy's bugprone-argument-comment check) to automatically verify
that the inline comment matches the parameter name in the function declaration,
preventing comments from drifting out of date or masking transposed arguments.

## Examples

Bad:

```cpp
get_active_gf_inverval_range(frame_info, rc, arf_active_or_kf, show_idx,
                             /*active_worst_quality*/ 0,
                             /*last_boosted_qindex*/ 0);
```

Good:

```cpp
get_active_gf_inverval_range(frame_info, rc, arf_active_or_kf, show_idx,
                             /*active_worst_quality=*/0,
                             /*last_boosted_qindex=*/0);
```
