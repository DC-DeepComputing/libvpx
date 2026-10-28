---
name: libvpx/expand-optimization-warning-suppressions-to-function
description: 'When suppressing optimization-dependent compiler warnings with #pragma GCC diagnostic, wrap the entire function if similar operations occur across multiple branches'
rubric: correctness
severity: should_fix
---

**Rule.** When suppressing optimization-dependent compiler warning false
positives (such as -Wstringop-overflow, -Warray-bounds, or
-Wmaybe-uninitialized) using #pragma GCC diagnostic push/ignored/pop, wrap the
suppression around the entire function if similar operations or memory accesses
occur across multiple cases or branches. Avoid scoping the suppression to a
single branch when sibling branches could trigger the same warning under
different compiler versions or optimization levels.

**Why.** Compiler optimization and dataflow analysis passes vary across compiler
versions, target architectures, and optimization flags (-O2 vs. -O3). Narrowly
suppressing a false positive in only one branch risks causing unexpected
warnings or -Werror build failures in other branches under different build
configurations.

## Examples

Bad:

```cpp
switch (tx_size) {
  case TX_8X8:
    for (i = 0; i < num_4x4_w; i += 2)
      t_above[i] = !!*(const uint16_t *)&above[i];
    break;
#if defined(__GNUC__) && !defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstringop-overflow"
#endif
  case TX_16X16:
    for (i = 0; i < num_4x4_w; i += 4)
      t_above[i] = !!*(const uint32_t *)&above[i];
    break;
#if defined(__GNUC__) && !defined(__clang__)
#pragma GCC diagnostic pop
#endif
  default:
    for (i = 0; i < num_4x4_w; i += 8)
      t_above[i] = !!*(const uint64_t *)&above[i];
    break;
}
```

Good:

```cpp
#if defined(__GNUC__) && !defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstringop-overflow"
#endif
void vp9_get_entropy_contexts(BLOCK_SIZE bsize, TX_SIZE tx_size, ...) {
  ...
  switch (tx_size) {
    case TX_8X8:
      for (i = 0; i < num_4x4_w; i += 2)
        t_above[i] = !!*(const uint16_t *)&above[i];
      break;
    case TX_16X16:
      for (i = 0; i < num_4x4_w; i += 4)
        t_above[i] = !!*(const uint32_t *)&above[i];
      break;
    default:
      for (i = 0; i < num_4x4_w; i += 8)
        t_above[i] = !!*(const uint64_t *)&above[i];
      break;
  }
}
#if defined(__GNUC__) && !defined(__clang__)
#pragma GCC diagnostic pop
#endif
```
