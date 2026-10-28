---
name: libvpx/avoid-variable-name-index-shadowing
description: When declaring local variables or function parameters, avoid names like index or standard C library functions to prevent -Wshadow compiler warnings
rubric: readability
severity: should_fix
---

**Rule.** Avoid naming local variables, function parameters, or struct members
after standard C library functions (such as 'index', 'gamma', 'y0', 'y1', 'yn',
'j0', 'j1', or 'jn') to prevent -Wshadow compiler warnings. Use more descriptive
or prefixed identifiers (such as 'frame_index', 'y_index', or 'idx') instead.

**Why.** libvpx enables -Wshadow during compilation. Standard C and POSIX
headers declare global functions such as index(3) in <strings.h> or Bessel
functions in <math.h>, causing GCC and Clang to emit shadowing warnings when
local variables share those names.

## Examples

Bad:

```cpp
// Bad: 'index' shadows standard/POSIX strings.h index() function under -Wshadow
int index = 0;
for (int i = 0; i < count; ++i) {
  if (match(i)) {
    index = i;
    break;
  }
}
```

Good:

```cpp
// Good: Use descriptive name with context prefix or idiomatic abbreviation
int y_index = 0;
for (int i = 0; i < count; ++i) {
  if (match(i)) {
    y_index = i;
    break;
  }
}
```
