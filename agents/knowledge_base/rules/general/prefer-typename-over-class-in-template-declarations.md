---
name: libvpx/prefer-typename-over-class-in-template-declarations
description: When declaring C++ template type parameters, use typename instead of class in the parameter list (e.g., template <typename T>)
rubric: readability
severity: nit
---

**Rule.** When declaring template type parameters, use `typename` rather than
`class` in the template parameter list (for example, `template <typename T>`).
Reserve `class` and `struct` for defining class and struct types.

**Why.** In C++ template parameter declarations, `typename` clearly signals that
the parameter represents any type (including primitives, pointers, and aliases)
rather than strictly a class or struct type. Using `typename` consistently
aligns with modern C++ style across the codebase.

## Examples

Bad:

```cpp
template <class T>
const T *GetVectorData(const std::vector<T> &v) {
  return v.data();
}
```

Good:

```cpp
template <typename T>
const T *GetVectorData(const std::vector<T> &v) {
  return v.data();
}
```
