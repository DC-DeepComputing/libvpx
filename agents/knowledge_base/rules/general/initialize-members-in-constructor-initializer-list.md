---
name: libvpx/initialize-members-in-constructor-initializer-list
description: When defining C++ constructors that initialize class member variables, use constructor member initializer lists rather than assigning members inside the constructor body.
rubric: readability
severity: nit
---

**Rule.** In C++ class constructors, initialize member variables using
constructor member initializer lists (or in-class default member initializers)
rather than assigning them inside the constructor body.

**Why.** Initializing members in constructor initializer lists directly
constructs member variables with their intended values, avoiding redundant
default-initialization followed by assignment in the body, and ensuring members
(including const and reference fields) are correctly initialized.

## Examples

Bad:

```cpp
class ResizingVideoSource {
 public:
  ResizingVideoSource(int width, int height) {
    top_width_ = width;
    top_height_ = height;
  }

 private:
  int top_width_;
  int top_height_;
};
```

Good:

```cpp
class ResizingVideoSource {
 public:
  ResizingVideoSource(int width, int height)
      : top_width_(width), top_height_(height) {}

 private:
  int top_width_;
  int top_height_;
};
```
