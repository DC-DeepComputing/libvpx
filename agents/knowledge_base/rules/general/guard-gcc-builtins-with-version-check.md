---
name: libvpx/guard-gcc-builtins-with-version-check
description: When guarding GCC builtins such as __builtin_ctz or __builtin_clz, check the GCC version is at least 3.4 rather than solely checking defined(__GNUC__)
rubric: correctness
severity: should_fix
---

**Rule.** When guarding GCC builtin functions (such as `__builtin_ctz` or
`__builtin_clz`), verify that the GCC version is at least 3.4
(`defined(__GNUC__) && ((__GNUC__ == 3 && __GNUC_MINOR__ >= 4) || __GNUC__ >=
4)`) or use `__has_builtin` / `VPX_HAS_BUILTIN` where applicable, rather than
checking only `defined(__GNUC__)`. Alternatively, prefer using portability
wrappers from headers like `vpx_ports/bitops.h` (such as `get_lsb` and
`get_msb`) instead of invoking raw builtins directly.

**Why.** GCC builtins such as `__builtin_ctz` and `__builtin_clz` were
introduced in GCC 3.4; testing only `defined(__GNUC__)` succeeds on older GCC
versions that lack these builtins, causing compile errors and broken builds on
older toolchains.

## Examples

Bad:

```cpp
#if defined(__GNUC__)
static INLINE int get_lsb(unsigned int n) {
  return __builtin_ctz(n);
}
#endif
```

Good:

```cpp
#if defined(__GNUC__) && \
    ((__GNUC__ == 3 && __GNUC_MINOR__ >= 4) || __GNUC__ >= 4)
static INLINE int get_lsb(unsigned int n) {
  return __builtin_ctz(n);
}
#endif
```
