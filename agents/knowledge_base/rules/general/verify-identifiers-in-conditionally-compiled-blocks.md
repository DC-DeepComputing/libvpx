---
name: libvpx/verify-identifiers-in-conditionally-compiled-blocks
rubric: correctness
severity: blocking
---

**Rule.** When adding, modifying, or porting code inside conditionally compiled
preprocessor blocks (such as #if CONFIG_*), verify that all variable, struct,
type, and member names match the target codebase (e.g., cpi in libvpx instead of
ppi from libaom). Do not rely solely on default builds, as disabled
configuration blocks are not compiled unless their build flags are explicitly
enabled.

**Why.** Code enclosed in #if CONFIG_* blocks for non-default features (such as
CONFIG_INTERNAL_STATS or optional tools) is omitted during standard compilation.
Mismatched or foreign identifiers from ported code pass unnoticed in default CI
but break non-default and downstream build configurations.

## Examples

Bad:

```cpp
#if CONFIG_INTERNAL_STATS
assert(ppi->b_calculate_psnr == 1);
#endif
```

Good:

```cpp
#if CONFIG_INTERNAL_STATS
assert(cpi->b_calculate_psnr == 1);
#endif
```
