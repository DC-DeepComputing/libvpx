---
name: libvpx/prefer-range-based-for-loops
description: When iterating over standard containers or sequences sequentially in C++, prefer range-based for loops over verbose explicit iterator loops
rubric: readability
severity: nit
---

**Rule.** When iterating sequentially through a container or range in C++
without needing explicit iterator manipulations or index arithmetic, use
range-based for loops (e.g., for (const auto &item : container)) instead of
explicit iterator loops. Use const auto & for non-primitive or non-mutated
elements, and auto & when modifying elements in place.

**Why.** Range-based for loops reduce boilerplate, eliminate verbose iterator
declarations and type repetitions, and prevent iterator mismatch bugs while
improving readability.

## Examples

Bad:

```cpp
for (std::vector<FrameInfo>::const_iterator info = frame_info_list_.begin();
     info != frame_info_list_.end(); ++info) {
  const unsigned int frame = static_cast<unsigned int>(info->pts);
  ProcessFrame(frame);
}
```

Good:

```cpp
for (const auto &info : frame_info_list_) {
  const unsigned int frame = static_cast<unsigned int>(info.pts);
  ProcessFrame(frame);
}
```
