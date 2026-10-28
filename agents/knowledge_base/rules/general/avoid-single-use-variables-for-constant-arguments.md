---
name: libvpx/avoid-single-use-variables-for-constant-arguments
description: When passing constant, sentinel, or dummy arguments to a function, pass literals directly or use inline /*param=*/ comments instead of declaring single-use temporary variables
rubric: readability
severity: should_fix
---

**Rule.** Do not declare single-use temporary or dummy local variables solely to
pass constant, sentinel, or default values into function calls. Pass literals
directly at the call site, and use inline argument comments (e.g.,
/*param_name=*/value) when non-obvious literal arguments require clarification.

**Why.** Declaring throwaway dummy variables for literal arguments pollutes the
local scope with redundant names without adding safety. Passing literals
directly keeps code concise, while inline /*param=*/ comments document argument
semantics at the call site without dummy variable clutter.

## Examples

Bad:

```cpp
// Declaring single-use temporary or dummy variables for constants:
int frame_size = -1;
if (vp9_encodedframe_overshoot(cpi, frame_size, &q)) {
  // ...
}

int dummy = 0;
RANGE active_gf_interval = get_active_gf_inverval_range(
    frame_info, rc, arf_active_or_kf, show_idx, dummy, dummy);
```

Good:

```cpp
// Pass literals directly, using inline argument comments for clarification:
if (vp9_encodedframe_overshoot(cpi, -1, &q)) {
  // ...
}

RANGE active_gf_interval = get_active_gf_inverval_range(
    frame_info, rc, arf_active_or_kf, show_idx, /*active_worst_quality=*/0,
    /*last_boosted_qindex=*/0);
```
