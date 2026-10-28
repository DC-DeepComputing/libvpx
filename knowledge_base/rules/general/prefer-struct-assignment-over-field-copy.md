---
name: libvpx/prefer-struct-assignment-over-field-copy
description: When copying plain C or POD structs, use direct struct assignment (dst = src) instead of copying fields individually
rubric: readability
severity: nit
---

**Rule.** When copying all or most fields of a plain C or POD struct (including
array elements or dynamically allocated entries), assign the entire struct
directly (`dst = src` or `*dst = *src`). Do not manually copy individual struct
fields member-by-member.

**Why.** Direct struct assignment is more concise and readable, reduces
boilerplate, and prevents synchronization bugs where newly added or modified
struct fields are omitted during manual copying.

## Examples

Bad:

```cpp
for (i = 0; i < data->size; i++) {
  data->frame_stats_list[i].frame_width = frame_stats_list[i].frame_width;
  data->frame_stats_list[i].frame_height = frame_stats_list[i].frame_height;
  data->frame_stats_list[i].num_blocks = frame_stats_list[i].num_blocks;
  data->frame_stats_list[i].block_stats_list =
      frame_stats_list[i].block_stats_list;
}
```

Good:

```cpp
for (i = 0; i < data->size; i++) {
  data->frame_stats_list[i] = frame_stats_list[i];
}
```
