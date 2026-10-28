---
name: libvpx/descriptive-frame-counter-names
description: When naming interval or elapsed frame counters, use descriptive unit-prefixed names like frames_since_<event> instead of ambiguous names like last_<event>
rubric: readability
severity: should_fix
---

**Rule.** Name variables and struct fields that track elapsed frames or
intervals since or until an event with explicit unit-prefixed names such as
frames_since_<event> (e.g., frames_since_key, frames_since_last_drop_overshoot)
or frames_to_<event> (e.g., frames_to_key). Avoid ambiguous prefixes such as
last_<event> or prev_<event> that obscure whether the variable is an elapsed
counter, a frame index/timestamp, or a boolean flag.

**Why.** Identifier names like last_<event> are ambiguous because readers cannot
determine if the field represents a boolean state, a specific frame
timestamp/index, or an accumulated count. Prefixing with the unit and relative
temporal relation makes the purpose, measurement unit, and behavior immediately
obvious across encoder modules.

## Examples

Bad:

```cpp
// Ambiguous: unclear whether this is a boolean flag, frame index, or elapsed counter
int last_drop_overshoot;
cpi->last_drop_overshoot++;
```

Good:

```cpp
// Explicitly indicates unit (frames) and relative relation (elapsed count since event)
int frames_since_last_drop_overshoot;
cpi->frames_since_last_drop_overshoot++;
```
