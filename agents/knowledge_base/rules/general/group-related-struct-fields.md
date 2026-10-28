---
name: libvpx/group-related-struct-fields
description: When declaring or updating struct definitions, place closely related fields—such as dynamic buffer pointers and their dimension or capacity trackers—adjacent to each other
rubric: readability
severity: nit
---

**Rule.** When declaring or updating struct definitions, place closely related
fields adjacent to each other. In particular, keep dynamically allocated buffer
pointers directly adjacent to the dimension, count, stride, or capacity
variables tracking their sizing and lifecycle, rather than separating them with
unrelated members.

**Why.** Colocating coupled struct members makes data dependencies, buffer
sizing requirements, and resource lifetimes immediately apparent to readers. It
clarifies the relationship between dynamic memory and its tracking state,
reducing the risk of errors during struct refactorings.

## Examples

Bad:

```cpp
typedef struct TileDataEnc {
  int *row_base_thresh_freq_fact;
  MV firstpass_top_mv;
  int sb_rows;  // Allocation dimension tracker separated from buffer pointer
} TileDataEnc;
```

Good:

```cpp
typedef struct TileDataEnc {
  int *row_base_thresh_freq_fact;
  int sb_rows;  // Dimension tracker colocated with buffer pointer
  MV firstpass_top_mv;
} TileDataEnc;
```
