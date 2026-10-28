---
name: libvpx/prefer-empty-over-size-for-container-emptiness
description: When testing whether a C++ container or string is empty, use empty() instead of checking size() == 0, size() > 0, or !size()
rubric: readability
severity: nit
---

**Rule.** Use `container.empty()` (or `!container.empty()`) rather than
comparing `.size()` or `.length()` against 0 (such as `size() == 0`, `size() >
0`, `size() != 0`, or `!size()`) to test whether a C++ container or string is
empty. Calling `.empty()` directly communicates intent and provides consistent,
idiomatic emptiness checks across standard container types.

**Why.** Calling `.empty()` explicitly conveys the intent to check for emptiness
rather than determining a count. In addition, `.empty()` is standard across STL
and container-like abstractions, providing clear and idiomatic code readability.

## Examples

Bad:

```cpp
if (v.size() > 0) {
  return v.data();
}
if (items.size() == 0) {
  return nullptr;
}
```

Good:

```cpp
if (!v.empty()) {
  return v.data();
}
if (items.empty()) {
  return nullptr;
}
```
