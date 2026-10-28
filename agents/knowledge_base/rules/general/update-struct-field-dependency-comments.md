---
name: libvpx/update-struct-field-dependency-comments
description: When modifying functions whose doc comments list struct field dependencies, update the comments to include any newly added direct or transitive field accesses
rubric: readability
severity: should_fix
---

**Rule.** When modifying or extending functions whose doc comments explicitly
enumerate the struct fields or external state they depend on (such as static and
dynamic configuration parameters), update the comment to list any newly
introduced field dependencies. Include transitive dependencies accessed through
called helper functions.

**Why.** Explicitly documented struct field dependencies aid modularization and
future struct refactorings (such as separating static from dynamic state).
Leaving dependency lists incomplete hides data coupling and transitive helper
dependencies, misleading future refactoring efforts.

## Examples

Bad:

```cpp
/* get_gop_coding_frame_num() depends on several fields in RATE_CONTROL *rc as
 * follows.
 * Static fields:
 *   rc->static_scene_max_gf_interval
 *   rc->min_gf_interval
 */
int get_gop_coding_frame_num(const VP9_COMP *cpi, ...) {
  // Uses helper that transitively accesses twopass->sr_diff_factor and
  // twopass->zm_factor, but comment above only enumerates rc fields.
  ...
}
```

Good:

```cpp
/* get_gop_coding_frame_num() depends on several fields in RATE_CONTROL *rc as
 * follows.
 * Static fields:
 *   rc->static_scene_max_gf_interval
 *   rc->min_gf_interval
 *   twopass->sr_diff_factor
 *   twopass->sr_default_decay_limit
 *   twopass->zm_factor
 */
int get_gop_coding_frame_num(const VP9_COMP *cpi, ...) {
  ...
}
```
