---
name: libvpx/align-helper-return-values-with-semantics
description: When defining helper functions that query counts, sizes, or properties, ensure return values accurately reflect the function name and semantics across all modes
rubric: readability
severity: should_fix
---

**Rule.** Ensure helper functions return values that strictly match the semantic
definition implied by their name across all configuration modes (e.g., returning
1 for single-layer modes in a layer count helper rather than 0). Adjust
calculations at caller sites as needed instead of returning synthetic or
artificial values from the helper.

**Why.** Returning synthetic values that contradict a helper's name creates
misleading semantics, makes the helper non-reusable across other callers, and
invites subtle calculation bugs when callers expect an accurate property or
count.

## Examples

Bad:

```cpp
static int get_arf_layers(int multi_layer_arf, int max_layers,
                          int coding_frame_num) {
  if (multi_layer_arf) {
    /* ... count layers ... */
    return layers;
  } else {
    return 0;  // Misleading: single ARF mode has 1 layer, not 0
  }
}
```

Good:

```cpp
static int get_arf_layers(int multi_layer_arf, int max_layers,
                          int coding_frame_num) {
  if (multi_layer_arf) {
    /* ... count layers ... */
    return layers;
  } else {
    return 1;  // Accurately returns 1 layer for single ARF mode
  }
}
```
