---
name: libvpx/document-non-obvious-loop-and-bitmask-conditions
description: When writing loop conditions or bitmask checks that parse structured maps, flags, or encoded buffers, add an explanatory comment clarifying the boundary condition
rubric: readability
severity: nit
---

**Rule.** When writing while or for loops whose termination relies on
non-obvious bitmask checks, flag tests, or sentinel delimiters (such as scanning
structured maps, GOP layouts, or encoded bitstreams), add an explanatory comment
above or within the loop explaining the semantic meaning of the condition. Do
not rely solely on remote header declarations or test cases for readers to
understand boundary conditions.

**Why.** Bitmask expressions and compound delimiter conditions in parsing loops
are opaque without context explaining what bit state signifies the boundary.
Inline comments make scanning and traversal logic immediately clear without
forcing readers to cross-reference distant flag definitions or external test
files.

## Examples

Bad:

```cpp
while (static_cast<size_t>(end_show_index) < gop_map.size() &&
       (gop_map[end_show_index] & kGopMapFlagStart) == 0) {
  ++end_show_index;
}
```

Good:

```cpp
// (gop_map[end_show_index] & kGopMapFlagStart) == 0 means this frame is
// within the current GOP (scanning until the start of the next GOP).
while (static_cast<size_t>(end_show_index) < gop_map.size() &&
       (gop_map[end_show_index] & kGopMapFlagStart) == 0) {
  ++end_show_index;
}
```
