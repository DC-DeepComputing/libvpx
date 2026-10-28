---
name: libvpx/document-multidimensional-index-dimensions
description: When declaring multi-dimensional arrays, tables, or nested vector structures, document the semantic meaning or hierarchy of each index dimension in a comment
rubric: readability
severity: nit
---

**Rule.** When declaring multi-dimensional arrays, fixed lookup tables, or
nested vector/array structures (such as context models, probability tables, or
statistics count buffers), document the semantic meaning and hierarchy of each
index dimension in a preceding comment (e.g., `// [plane][tx_size][context]`).

**Why.** Multi-dimensional structures and nested vectors representing contextual
statistics or probabilities are opaque without documentation explaining what
each dimension index corresponds to. Clear dimension hierarchy comments prevent
off-by-one index misalignments and make data layout immediately understandable.

## Examples

Bad:

```cpp
struct FrameCounts {
  std::vector<std::vector<std::vector<unsigned int>>> y_mode;
  unsigned int uv_mode[INTRA_MODES][INTRA_MODES];
};
```

Good:

```cpp
struct FrameCounts {
  // Intra prediction mode counts for luma component:
  // [above_intra_mode][left_intra_mode][current_y_mode]
  std::vector<std::vector<std::vector<unsigned int>>> y_mode;

  // Intra prediction mode counts for chroma component:
  // [current_y_mode][current_uv_mode]
  unsigned int uv_mode[INTRA_MODES][INTRA_MODES];
};
```
