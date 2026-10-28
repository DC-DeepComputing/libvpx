---
name: libvpx/document-non-obvious-member-variables
description: When naming member variables or struct fields, use explicit, self-describing identifiers rather than ambiguous prefixes, and document non-obvious coordinate spaces or semantics
rubric: readability
severity: nit
---

**Rule.** Name class member variables and struct fields explicitly after what
they represent (e.g., configured_width_ or original_width_ for baseline
resolutions) instead of using ambiguous or context-borrowed prefixes like top_
that require comments. When member purpose, coordinate spaces, or lifetime
invariants cannot be made fully self-explanatory from the identifier and type
alone, add an explanatory comment clarifying their role.

**Why.** Ambiguous prefixes like top_ outside hierarchical multi-layer contexts
(such as SVC spatial layers) mislead readers and force reliance on comments.
Self-describing variable names make code immediately understandable, while
clarifying comments prevent misinterpretations of coordinate systems, units, and
lifecycle states.

## Examples

Bad:

```cpp
class ResizeTest : public ::testing::Test {
 protected:
  // top_width_/height_ is the configured resolution when codec is created.
  int top_width_;
  int top_height_;
  int cur_width_;
  int cur_height_;
};
```

Good:

```cpp
class ResizeTest : public ::testing::Test {
 protected:
  // Baseline resolution configured at codec creation.
  int configured_width_;
  int configured_height_;
  // Current active frame dimensions after dynamic scaling.
  int cur_width_;
  int cur_height_;
};
```
