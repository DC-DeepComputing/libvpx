---
name: libvpx/avoid-c99-printf-specifiers
description: When formatting strings with printf-style functions or vpx_internal_error, avoid C99 length modifiers like %z or %t; cast arguments and use %d or %u
rubric: correctness
severity: should_fix
---

**Rule.** Avoid using C99 format specifier length modifiers like '%t' (for
ptrdiff_t) or '%z' (for size_t) in printf-style format strings (such as in
vpx_internal_error, snprintf, or printf). Explicitly cast the argument (e.g. to
(int) or (unsigned int)) and use standard format specifiers like '%d' or '%u'.

**Why.** Older MSVC toolchains and non-C99-compliant C runtime libraries do not
support '%t' or '%z' length modifiers in printf format strings, causing runtime
format errors or corrupted output. Explicit casts to standard integer types
ensure cross-compiler portability.

## Examples

Bad:

```cpp
vpx_internal_error(&pbi->common.error, VPX_CODEC_CORRUPT_FRAME,
                   "Truncated packet or corrupt partition. No bytes left %td.",
                   bytes_left);
```

Good:

```cpp
vpx_internal_error(&pbi->common.error, VPX_CODEC_CORRUPT_FRAME,
                   "Truncated packet or corrupt partition. No bytes left %d.",
                   (int)bytes_left);
```
