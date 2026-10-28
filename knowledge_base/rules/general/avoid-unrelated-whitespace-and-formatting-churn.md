---
name: libvpx/avoid-unrelated-whitespace-and-formatting-churn
description: When modifying code for functional changes or bug fixes, avoid adding unrelated whitespace modifications, blank lines, or drive-by formatting edits
rubric: change_scope
severity: nit
---

**Rule.** Do not include unrelated whitespace adjustments, blank line additions
or deletions, or drive-by reformatting in patches focused on functional changes
or bug fixes. Keep diffs minimal and scoped strictly to necessary modifications
to avoid cluttering git blame and complicating code review.

**Why.** Unrelated whitespace and formatting modifications add noisy diff churn,
make code reviews more difficult by obscuring substantive changes, and pollute
git history/blame without providing functional or architectural value.

## Examples

Bad:

```cpp
// In a patch modifying codec control logic:
if (res != VPX_CODEC_OK) {
  return res;
}

// Unrelated blank line added above existing comment:
// There's no codec control for multiple alt-refs so check the encoder
if (cpi->oxcf.pass == 2 && cpi->oxcf.first_pass_alt_ref) {
  return VPX_CODEC_OK;
}
```

Good:

```cpp
// Keep existing formatting intact without drive-by whitespace churn:
if (res != VPX_CODEC_OK) {
  return res;
}
// There's no codec control for multiple alt-refs so check the encoder
if (cpi->oxcf.pass == 2 && cpi->oxcf.first_pass_alt_ref) {
  return VPX_CODEC_OK;
}
```
