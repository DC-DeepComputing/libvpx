---
name: libvpx/vpxenc-c/align-cli-arg-defs-with-control-maps
description: When defining CLI arguments mapped to codec controls in parallel arrays, keep mapped arguments index-aligned and place unmapped arguments at the end
rubric: correctness
severity: should_fix
---

**Rule.** In CLI tools using parallel argument and control ID arrays (such as
vp8_args/vp8_arg_ctrl_map and vp9_args/vp9_arg_ctrl_map in vpxenc.c), keep every
argument mapped to a codec control ID strictly index-aligned with its
corresponding control map entry. Place all arguments that do not map to a codec
control (or are handled separately, such as bitdeptharg) at the end of the
argument array after all mapped entries.

**Why.** In vpxenc.c, the CLI parser looks up codec control IDs in ctrl_args_map
using the array index of the matched option in ctrl_args. Placing unmapped
arguments before control-mapped arguments shifts subsequent array indices,
mapping options to the wrong codec control IDs or reading beyond the control map
bounds.

## Examples

Bad:

```cpp
static const arg_def_t *vp9_args[] = {
  &cpu_used_vp9,
  &row_mt,
#if CONFIG_VP9_HIGHBITDEPTH
  &bitdeptharg,
  &inbitdeptharg,
#endif
  &disable_loopfilter,
  NULL
};
```

Good:

```cpp
static const arg_def_t *vp9_args[] = {
  &cpu_used_vp9,
  &row_mt,
  &disable_loopfilter,
#if CONFIG_VP9_HIGHBITDEPTH
  &bitdeptharg,
  &inbitdeptharg,
#endif
  NULL
};
```
