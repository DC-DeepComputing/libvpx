---
name: libvpx/initialize-indices-to-starting-value
description: When declaring index or counter variables, initialize them directly to their starting value instead of initializing to zero and immediately incrementing before first use
rubric: readability
severity: nit
---

**Rule.** Do not initialize an index, offset, or counter variable to zero and
immediately increment it unconditionally before its first use. Declare and
initialize the variable directly to its true starting value at the point where
its usage begins.

**Why.** Initializing a variable with a dummy zero value only to mutate it
immediately before any reads introduces unnecessary mutation and obscures the
variable's true initial value. Initializing directly to the starting value where
tracking begins improves readability and clarifies intent.

## Examples

Bad:

```cpp
int frame_index = 0;
if (key_frame) {
  gf_group_set_key_frame(gf_group, 0);
} else {
  gf_group_set_overlay_frame(gf_group, 0);
}
++frame_index;
```

Good:

```cpp
if (key_frame) {
  gf_group_set_key_frame(gf_group, 0);
} else {
  gf_group_set_overlay_frame(gf_group, 0);
}
int frame_index = 1;
```
