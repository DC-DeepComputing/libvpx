---
name: libvpx/avoid-duplicate-access-specifiers
description: 'When declaring C++ classes or structs, avoid redundant consecutive or duplicate access specifiers such as repeating public: or private: in the same section'
rubric: readability
severity: nit
---

**Rule.** Do not declare redundant consecutive or duplicate access specifiers
(such as repeating private:, protected:, or public:) within C++ class or struct
definitions. Group member declarations under a single access specifier label
rather than repeating the specifier.

**Why.** Repeating the same access specifier without an intervening change in
access level adds visual noise and clutter without altering member visibility or
semantics.

## Examples

Bad:

```cpp
class VP9RateControlRTC {
 private:
  VP9RateControlRTC() {}

 private:
  VP9_COMP *cpi_;
};
```

Good:

```cpp
class VP9RateControlRTC {
 private:
  VP9RateControlRTC() {}
  VP9_COMP *cpi_;
};
```
