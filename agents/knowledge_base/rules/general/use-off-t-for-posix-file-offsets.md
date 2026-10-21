---
name: libvpx/use-off-t-for-posix-file-offsets
rubric: correctness
severity: blocking
---

**Rule.** When defining type aliases (such as `FileOffset`) and macro shims for
64-bit file seeking functions (`fseeko`, `ftello`, `fseeko64`, `ftello64`,
`_fseeki64`, `_ftelli64`), consolidate all platform and build configurations
into a single `#if`/`#elif`/`#else` conditional block. Match the exact offset
type expected by each platform (`int64_t` for MSVC `_MSC_VER`, `off64_t` for
MinGW `_WIN32`, `off_t` for POSIX) rather than generic `long` or `long long`.
Guard the default POSIX branch with `#elif CONFIG_OS_SUPPORT` rather than an
unconditional `#else` so that no-OS/bare-metal builds (`!CONFIG_OS_SUPPORT`)
cleanly fall back to standard `long` and `fseek`/`ftell` without duplicate
typedefs or missing headers.

**Why.** Primitive integer types like `long` and `long long` differ in size and
signedness across platforms (e.g., `long` is 32-bit on Win64), causing type
mismatch warnings or truncation when seeking in large files. Consolidating all
targets into a single preprocessor block with `#elif CONFIG_OS_SUPPORT` prevents
duplicate typedefs and header inclusions in bare-metal configurations while
ensuring full 64-bit seeking support across all supported OS platforms.

## Examples

Bad:

```cpp
/* Split preprocessor blocks and incorrect platform types */
#if defined(_MSC_VER)
#define fseeko _fseeki64
#define ftello _ftelli64
typedef int64_t FileOffset;
#elif defined(_WIN32)
#define fseeko fseeko64
#define ftello ftello64
typedef long long FileOffset; /* Incorrect type for MinGW */
#else
typedef off_t FileOffset; /* Unconditional branch conflicts with fallback */
#endif

#if !CONFIG_OS_SUPPORT
#define fseeko fseek
#define ftello ftell
typedef long FileOffset;
#endif
```

Good:

```cpp
/* Consolidated preprocessor block matching platform types and CONFIG_OS_SUPPORT */
#if defined(_MSC_VER)
#define fseeko _fseeki64
#define ftello _ftelli64
typedef int64_t FileOffset;
#elif defined(_WIN32)
#define fseeko fseeko64
#define ftello ftello64
typedef off_t FileOffset;
#elif CONFIG_OS_SUPPORT
#define fseeko fseeko
#define ftello ftello
typedef off_t FileOffset;
#else
#define fseeko fseek
#define ftello ftell
typedef long FileOffset; /* NOLINT */
#endif
```
