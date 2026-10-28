---
name: libvpx/concise-helper-function-names-without-redundant-qualifiers
description: When naming internal static or helper functions, use concise descriptive names and omit redundant directional or implementation qualifiers like _from_below
rubric: readability
severity: nit
---

**Rule.** Prefer concise, descriptive names for static helper functions and
utilities (e.g., clamp_framerate) rather than appending redundant directional or
implementation qualifiers (e.g., clamp_framerate_from_below,
filter_above_threshold) when function behavior is clear and no multiple variants
exist in the same scope.

**Why.** Overly verbose suffixes and directional qualifiers add visual noise
without improving clarity when the function's scope, parameters, and logic are
straightforward. Concise names follow established libvpx conventions and keep
call sites clean.

## Examples

Bad:

```cpp
static double clamp_framerate_from_below(double framerate) {
  if (framerate < .1) {
    return 30.0;
  }
  return framerate;
}
```

Good:

```cpp
static double clamp_framerate(double framerate) {
  if (framerate < .1) {
    return 30.0;
  }
  return framerate;
}
```
