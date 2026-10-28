---
name: libvpx/dimension-fixed-arrays-to-exact-count
description: When declaring fixed-size arrays or buffers, dimension them to the exact element count or designated capacity constant rather than arbitrary or off-by-one sizes
rubric: readability
severity: should_fix
---

**Rule.** When declaring fixed-size member arrays, stack buffers, or lookup
tables, dimension them to match the exact number of elements used or use a
designated capacity constant (such as VPX_MAX_LAYERS). Avoid arbitrary or
off-by-one dimensions that allocate unused slots and mislead readers about the
supported configurations.

**Why.** Arbitrary or off-by-one array bounds mislead maintainers about the
actual capacity or configuration limit and can mask out-of-bounds access
assumptions or waste stack and struct storage.

## Examples

Bad:

```cpp
// Sizing an array with an extra unused element or arbitrary dimension:
// (e.g., 10 for a 3 spatial x 3 temporal layer configuration)
int bitrate_layer_[10];
```

Good:

```cpp
// Dimensioned to the exact required layer count (3 spatial x 3 temporal = 9):
int bitrate_layer_[9];
// Or sized using a designated capacity constant:
int layer_rates_[VPX_MAX_LAYERS];
```
