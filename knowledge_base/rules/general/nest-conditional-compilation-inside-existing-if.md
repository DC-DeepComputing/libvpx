---
name: libvpx/nest-conditional-compilation-inside-existing-if
description: 'When adding conditionally compiled logic for a runtime condition, nest the #if directive inside the existing if block rather than duplicating consecutive if statements'
rubric: readability
severity: should_fix
---

**Rule.** When adding conditionally compiled code (#if / #ifdef) that executes
under an existing runtime condition, place the preprocessor directives inside
the existing if (condition) block. Do not introduce a separate, consecutive if
(condition) statement solely to wrap the conditionally compiled logic.

**Why.** Duplicating consecutive if statements for the same runtime condition
adds redundant branching logic and harms readability. Nesting the preprocessor
conditional within the existing runtime if block keeps all logic for that
condition consolidated under a single branch.

## Examples

Bad:

```cpp
#if CONFIG_BETTER_HW_COMPATIBILITY
if (is_scaled) {
  assert(xd->mi[0]->mbmi.sb_type != BLOCK_4X8 &&
         xd->mi[0]->mbmi.sb_type != BLOCK_8X4);
}
#endif

if (is_scaled) {
  const int x_start = (-xd->mb_to_left_edge >> (3 + pd->subsampling_x));
  ...
}
```

Good:

```cpp
if (is_scaled) {
  const int x_start = (-xd->mb_to_left_edge >> (3 + pd->subsampling_x));
  const int y_start = (-xd->mb_to_top_edge >> (3 + pd->subsampling_y));
#if CONFIG_BETTER_HW_COMPATIBILITY
  assert(xd->mi[0]->mbmi.sb_type != BLOCK_4X8 &&
         xd->mi[0]->mbmi.sb_type != BLOCK_8X4);
#endif
  ...
}
```
