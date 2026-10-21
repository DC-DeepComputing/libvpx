---
name: libvpx/bind-range-for-elements-by-const-ref
rubric: safety
severity: should_fix
---

**Rule.** When iterating over containers of structs, classes, or non-primitive
objects in C++ range-based for loops, bind loop variables by const auto & (or
auto & if mutation is required) rather than copying elements by value. Reserve
by-value binding for primitive scalar types such as integers, floats, pointers,
and small enums.

**Why.** Binding non-primitive container elements by value creates an
unnecessary copy of each element on every loop iteration, introducing
performance and copy overhead. Binding by const reference avoids copying and
accesses the underlying container elements directly.

## Examples

Bad:

```cpp
// Copies each non-primitive struct/object on every iteration:
for (auto config : encode_config_list) {
  UpdateEncodeConfig(config, &oxcf);
}
```

Good:

```cpp
// Binds by const reference to avoid unnecessary copies:
for (const auto &config : encode_config_list) {
  UpdateEncodeConfig(config, &oxcf);
}
```
