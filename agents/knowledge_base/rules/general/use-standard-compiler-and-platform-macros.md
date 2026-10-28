---
name: libvpx/use-standard-compiler-and-platform-macros
description: When writing platform or compiler conditional guards, use standard predefined macros like _MSC_VER or _WIN32 rather than external framework macros like OS_WIN
rubric: correctness
severity: blocking
---

**Rule.** When guarding compiler-specific workarounds or platform-specific code,
use standard predefined macros (such as `_MSC_VER`, `__GNUC__`, `_WIN32`,
`__APPLE__`, or `__linux__`) rather than external framework macros (such as
Chromium's `OS_WIN` or `BUILDFLAG(...)`). Ensure compiler-specific quirks (such
as MSVC bitfield limitations) are guarded specifically by compiler macros like
`#if defined(_MSC_VER)` rather than OS macros.

**Why.** libvpx is an independent library that does not include external
framework headers or Chromium build configuration macros like `OS_WIN`. Using
undefined external macros causes conditional blocks to silently fail to activate
or miscompile, and guarding compiler-specific syntax with OS macros breaks
alternative compilers on that platform.

## Examples

Bad:

```cpp
#if defined(OS_WIN)
#define COMPILE_TIME_ASSERT(boolexp)              \
  do {                                            \
    char compile_time_assert[(boolexp) ? 1 : -1]; \
    (void)compile_time_assert;                    \
  } while (0)
#else /* !OS_WIN */
#define COMPILE_TIME_ASSERT(boolexp)                         \
  do {                                                       \
    struct {                                                 \
      unsigned int compile_time_assert : (boolexp) ? 1 : -1; \
    } compile_time_assert;                                   \
    (void)compile_time_assert;                               \
  } while (0)
#endif /* OS_WIN */
```

Good:

```cpp
#if defined(_MSC_VER)
#define COMPILE_TIME_ASSERT(boolexp)              \
  do {                                            \
    char compile_time_assert[(boolexp) ? 1 : -1]; \
    (void)compile_time_assert;                    \
  } while (0)
#else /* !_MSC_VER */
#define COMPILE_TIME_ASSERT(boolexp)                         \
  do {                                                       \
    struct {                                                 \
      unsigned int compile_time_assert : (boolexp) ? 1 : -1; \
    } compile_time_assert;                                   \
    (void)compile_time_assert;                               \
  } while (0)
#endif /* _MSC_VER */
```
