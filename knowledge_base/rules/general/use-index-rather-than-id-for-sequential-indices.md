---
name: libvpx/use-index-rather-than-id-for-sequential-indices
description: When naming sequential zero-based counters, offsets, or positions, use index or idx rather than id, and qualify scope when disambiguating
rubric: readability
severity: should_fix
---

**Rule.** When naming struct members, variables, or function arguments
representing sequential zero-based indices, offsets, or position counters, name
them with `index` or `idx` rather than `id`. Reserve `id` for opaque,
non-sequential, or globally unique identifiers. When disambiguating multiple
index levels or scopes of the same entity (such as local vs. global position),
qualify the index identifier (e.g., `global_index` vs. `local_index`) rather
than switching terminology to `id`.

**Why.** An `id` suggests an arbitrary or opaque unique identifier (such as an
entity UUID, hardware device handle, or codec control ID) rather than an ordered
position in a sequence. Using `index` or `idx` communicates zero-based
sequential counting and array-indexing semantics, while scope qualifiers
maintain consistency without semantic ambiguity.

## Examples

Bad:

```cpp
typedef struct vpx_rc_gop_info {
  /*!
   * The index of the current gop, starts from zero, resets to zero
   * when a keyframe is set.
   */
  int gop_id;
} vpx_rc_gop_info_t;
```

Good:

```cpp
typedef struct vpx_rc_gop_info {
  /*!
   * The index of the current gop, starts from zero, resets to zero
   * when a keyframe is set.
   */
  int gop_global_index;
} vpx_rc_gop_info_t;
```
