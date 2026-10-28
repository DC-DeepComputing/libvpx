---
name: libvpx/prefer-struct-over-positional-array
description: When defining multi-field metrics or records in structs/APIs, use dedicated structs with named members instead of positional raw arrays
rubric: safety
severity: should_fix
---

**Rule.** Do not use fixed-size raw arrays with positional indices (such as
double (*)[N] or multi-dimensional raw arrays) to store or pass collections of
heterogeneous fields, metrics, or features. Define a dedicated struct with named
members for each field and pass or store arrays/pointers of that struct type
instead. If raw arrays must be retained for ABI compatibility, thoroughly
document array layout and element semantics with comments.

**Why.** Positional indexing into fixed-size raw arrays for heterogeneous fields
obscures member semantics, units, and ordering, making API contracts fragile and
prone to out-of-bounds access or buffer overflow when fields are modified.
Dedicated structs provide compiler-checked member access, self-documenting field
semantics, and safe API extensibility.

## Examples

Bad:

```cpp
typedef struct vpx_rc_firstpass_stats {
  /* Undocumented raw array where positional indices represent distinct metrics */
  double (*frame_stats)[25];
  int num_frames;
} vpx_rc_firstpass_stats_t;
```

Good:

```cpp
typedef struct vpx_rc_frame_stats {
  double frame;
  double weight;
  double intra_error;
  double coded_error;
  /* ... named metrics ... */
} vpx_rc_frame_stats_t;

typedef struct vpx_rc_firstpass_stats {
  /*! Pointer to array of per-frame statistics records. */
  vpx_rc_frame_stats_t *frame_stats;
  int num_frames;
} vpx_rc_firstpass_stats_t;
```
