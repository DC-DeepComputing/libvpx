---
name: libvpx/declare-each-variable-on-its-own-line
description: When declaring variables, place each variable declaration on its own line instead of combining multiple variables—especially mixing initialized and uninitialized variables—into one statement
rubric: readability
severity: nit
---

**Rule.** Do not declare multiple variables in a single comma-separated
statement on the same line, particularly when mixing initialized and
uninitialized variables. Declare each variable in its own statement on a
separate line.

**Why.** Declaring multiple variables on a single line obscures variable types
and initialization state, making it easy to miss uninitialized variables or
misread pointer decorations. Separate declarations improve readability and
ensure cleaner diffs when initializers change.

## Examples

Bad:

```cpp
int frame_idx = 0, i;
```

Good:

```cpp
int frame_idx = 0;
int i;
```
