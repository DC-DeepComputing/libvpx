---
name: libvpx/avoid-redundant-this-member-access
description: When accessing member variables or functions in C++, omit redundant this-> prefixes unless required for dependent template lookup or disambiguation
rubric: readability
severity: nit
---

**Rule.** Do not use redundant `this->` prefixes when accessing member variables
or calling member functions in non-template C++ classes. Omit `this->` unless
explicitly required for dependent name lookup in templated base classes or to
disambiguate shadowed parameters.

**Why.** In standard C++ class member functions, member variables and methods
are directly accessible within scope, and member variables are already
distinguished by naming conventions such as trailing underscores. Explicit
`this->` qualifications add visual clutter without providing semantic benefit.

## Examples

Bad:

```cpp
void SimpleEncode::CloseFile() {
  if (this->in_file_ != nullptr) {
    fclose(this->in_file_);
    this->in_file_ = nullptr;
  }
}
```

Good:

```cpp
void SimpleEncode::CloseFile() {
  if (in_file_ != nullptr) {
    fclose(in_file_);
    in_file_ = nullptr;
  }
}
```
