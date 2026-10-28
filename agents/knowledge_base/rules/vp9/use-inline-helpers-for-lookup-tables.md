---
name: libvpx/vp9/use-inline-helpers-for-lookup-tables
description: When querying codec block properties or lookup tables, call dedicated inline helpers like get_subsize() instead of directly indexing raw lookup arrays
rubric: readability
severity: should_fix
---

**Rule.** Use existing inline accessor functions (such as `get_subsize(bsize,
partition)`) rather than indexing underlying multidimensional or flat lookup
tables directly (such as `subsize_lookup[partition][bsize]`). Verify whether an
inline helper encapsulates table access before writing direct array indexing.

**Why.** Directly indexing multi-dimensional lookup tables is error-prone due to
swapped index dimensions (e.g., `[partition][bsize]` vs. `[bsize][partition]`)
and couples callers to internal table layout. Dedicated inline helpers
encapsulate the index order cleanly with zero runtime overhead.

## Examples

Bad:

```cpp
// Direct indexing into multi-dimensional lookup table
subsize = subsize_lookup[partition][bsize];
```

Good:

```cpp
// Use existing inline helper function
subsize = get_subsize(bsize, partition);
```
