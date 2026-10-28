---
name: libvpx/general-simplify-negated-conjunction-to-negated-disjunction
description: When checking that multiple conditions are all false, simplify conjunctions of negations (!A && !B) into a single negated disjunction !(A || B)
rubric: readability
severity: nit
---

**Rule.** When checking that multiple conditions or flags are all false,
simplify conjunctions of negated terms (`!A && !B`) into a single negated
disjunction (`!(A || B)`). Do not chain multiple separate logical NOT operators
with `&&` when expressing that neither condition holds.

**Why.** Expressions of the form `!(A || B)` are easier to read and mentally
evaluate than multiple negations joined by `&&` (`!A && !B`). Using De Morgan's
laws reduces visual noise and clarifies the intent that the block executes only
when none of the specified conditions are met.

## Examples

Bad:

```cpp
// Multiple separate negations joined by logical AND
if (!(cpi->ref_frame_flags & VP9_LAST_FLAG) &&
    !cpi->ext_refresh_last_frame) {
  cpi->lst_fb_idx = map_idx;
}

if (!is_key_frame && !is_intra_only) {
  return VPX_CODEC_ERROR;
}
```

Good:

```cpp
// Simplified to a single negated disjunction
if (!(cpi->ref_frame_flags & VP9_LAST_FLAG ||
      cpi->ext_refresh_last_frame)) {
  cpi->lst_fb_idx = map_idx;
}

if (!(is_key_frame || is_intra_only)) {
  return VPX_CODEC_ERROR;
}
```
