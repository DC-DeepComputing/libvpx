---
name: libvpx/consistent-identifier-naming-in-structs-and-apis
description: When declaring members or parameters within a struct, class, or API, maintain consistent naming without mixing abbreviations like _idx with spelled-out terms like _index.
rubric: readability
severity: should_fix
---

**Rule.** Maintain consistent naming conventions and terminology across related
identifiers within the same struct, class, function signature, or API. Do not
mix abbreviated terms (such as _idx, _cnt, _num, _ptr, _len) with their
spelled-out equivalents (such as _index, _count, _number, _pointer, _length)
within the same definition or interface.

**Why.** Inconsistent naming within a single struct or API creates confusion for
callers and maintainers, makes code harder to search and autocomplete, and
violates interface symmetry.

## Examples

Bad:

```cpp
struct GroupOfPicture {
  int encode_frame_index;
  int show_frame_count;
  int start_show_idx;
};
```

Good:

```cpp
struct GroupOfPicture {
  int encode_frame_index;
  int show_frame_count;
  int start_show_index;
};
```
