---
name: libvpx/predicate-naming-for-boolean-flags-and-fields
description: When naming boolean flags, state variables, or mode settings in structs, use predicate prefixes (is_*, has_*) or *_mode suffixes instead of bare nouns
rubric: readability
severity: nit
---

**Rule.** Name boolean flags, state variables, and struct fields representing
binary conditions using predicate forms (such as `is_*`, `has_*`, or `allow_*`),
and suffix mode configuration fields with `_mode` (such as `simulcast_mode` or
`denoiser_mode`), rather than using ambiguous bare nouns (such as `usage`,
`flag`, or `simulcast`). Ensure every field's identifier clearly communicates
whether it represents a binary predicate condition, an operational mode setting,
or a numeric value.

**Why.** Bare noun identifiers like `usage` or `simulcast` are ambiguous because
they obscure whether a field is a boolean condition, an operational mode
setting, or a numeric count/data buffer. Using predicate prefixes for booleans
and `_mode` suffixes for mode flags explicitly conveys their semantics at both
declaration and use sites.

## Examples

Bad:

```cpp
struct BUFFER_LONGTERM_REF {
  int idx;
  int usage;  // Ambiguous noun: boolean condition or reference count?
};

typedef struct SVC {
  int simulcast;  // Ambiguous noun: operational mode flag or stream count?
} SVC;
```

Good:

```cpp
struct BUFFER_LONGTERM_REF {
  int idx;
  int is_used;  // Clear boolean predicate
};

typedef struct SVC {
  int simulcast_mode;  // Explicit mode configuration
} SVC;
```
