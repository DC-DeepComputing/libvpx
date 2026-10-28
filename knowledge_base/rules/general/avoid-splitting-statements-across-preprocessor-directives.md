---
name: libvpx/avoid-splitting-statements-across-preprocessor-directives
description: 'When using preprocessor #if/#else conditional compilation directives, do not split syntactic statements or opening braces across branches; keep statements and blocks complete within each branch.'
rubric: readability
severity: nit
---

**Rule.** Do not split C/C++ syntactic constructs—such as if/while/for condition
headers or opening braces—across preprocessor #if, #elif, #else, or #endif
directives. Keep statements and blocks syntactically complete within each
conditional branch, or compute condition sub-expressions into helper variables
before the statement.

**Why.** Splitting condition headers or opening braces across #if/#else
directives while sharing a single closing brace harms readability, breaks editor
syntax highlighting and brace matching, and interferes with automated formatting
tools.

## Examples

Bad:

```cpp
#if ULONG_MAX > INT64_MAX
if (duration > INT64_MAX || pts > INT64_MAX - (int64_t)duration) {
#else
if (pts > INT64_MAX - duration) {
#endif
  vpx_internal_error(&cpi->common.error, VPX_CODEC_INVALID_PARAM,
                     "duration is too big");
}
```

Good:

```cpp
#if ULONG_MAX > INT64_MAX
if (duration > INT64_MAX) {
  vpx_internal_error(&cpi->common.error, VPX_CODEC_INVALID_PARAM,
                     "duration is too big");
}
#endif
if (pts > INT64_MAX - (int64_t)duration) {
  vpx_internal_error(&cpi->common.error, VPX_CODEC_INVALID_PARAM,
                     "relative pts + duration is too big");
}
```
