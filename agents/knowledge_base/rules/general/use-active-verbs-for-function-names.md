---
name: libvpx/use-active-verbs-for-function-names
description: When naming functions or methods that perform operations, transformations, or state mutations, use active verb phrases rather than bare nouns, adjectives, or lifecycle labels
rubric: readability
severity: should_fix
---

**Rule.** Name functions and methods that perform actions, operations, data
transformations, or state mutations using active verbs or verb phrases (e.g.,
invert, compute, transform) rather than adjectives or standalone nouns (e.g.,
inverse, calculation). Additionally, when naming interface methods or functions
that execute following a processing stage or lifecycle phase, include an
explicit action verb phrase (such as PostEncodeUpdate or PreEncodeProcess)
rather than a bare phase label (such as PostEncode or PreEncode) to clearly
distinguish active mutation operations from passive event notifications or
lifecycle hooks.

**Why.** Function and method names using active verbs clearly communicate that
an operation, computation, or state mutation is being executed at declaration
and call sites. Adjectives or nouns make calls read like property accesses or
data types, while bare lifecycle labels (like PostEncode or PreProcess) sound
like passive event hooks or callbacks and obscure whether the method modifies
state or processes data.

## Examples

Bad:

```cpp
// 'inverse' is an adjective/noun rather than an action verb for a transformation.
static INLINE vpx_rational_t inverse_vpx_rational(vpx_rational_t v) {
  const vpx_rational_t inv = { v.den, v.num };
  return inv;
}

class RateControlRTC {
 public:
  // Ambiguous: sounds like a passive lifecycle hook rather than a state mutation.
  void PostEncode(uint64_t encoded_frame_size);
};
```

Good:

```cpp
// 'invert' is an active verb describing the transformation performed.
static INLINE vpx_rational_t invert_vpx_rational(vpx_rational_t v) {
  const vpx_rational_t inv = { v.den, v.num };
  return inv;
}

class RateControlRTC {
 public:
  // Explicit action verb signals that the method mutates rate control state.
  void PostEncodeUpdate(uint64_t encoded_frame_size);
};
```
