---
name: libvpx/vp9/bypass-decision-logic-only-on-active-runtime-override
description: When integrating external command or feature overrides, do not compile out standard logic with `#if`; bypass defaults dynamically only when runtime overrides are active
rubric: correctness
severity: should_fix
---

**Rule.** Do not unconditionally disable or compile out standard decision,
heuristic, or rate-control logic under build-configuration macros (such as `#if
!CONFIG_*`) when adding external command overrides. Instead, keep default logic
compiled in and bypass it dynamically only when runtime override structures or
flags (such as `encode_command->use_external_arf` or `cpi->ext_ratectrl.ready`)
are explicitly active.

**Why.** Unconditionally compiling out standard decision logic under a build
flag bypasses default heuristics for all runs in that build configuration, even
when no external override is provided at runtime. Dynamically checking the
active override struct ensures default behavior is preserved unless an external
override is actively requested.

## Examples

Bad:

```cpp
// Disables standard alt-ref heuristic for all builds with CONFIG_RATE_CTRL
#if !CONFIG_RATE_CTRL
*use_alt_ref &= zero_motion_accumulator < 0.995;
#endif  // CONFIG_RATE_CTRL
```

Good:

```cpp
// Evaluates standard alt-ref heuristic whenever external override is absent
if (encode_command == NULL || !encode_command->use_external_arf) {
  *use_alt_ref &= zero_motion_accumulator < 0.995;
}
```
