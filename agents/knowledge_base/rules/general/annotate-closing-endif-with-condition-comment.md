---
name: libvpx/annotate-closing-endif-with-condition-comment
description: 'When closing or branching non-trivial preprocessor conditional blocks (#if, #ifdef, #ifndef), annotate #endif and #else directives with trailing comments matching the condition'
rubric: readability
severity: nit
---

**Rule.** Annotate `#endif` and `#else` directives that terminate or branch
non-trivial or multi-line preprocessor conditional compilation blocks (`#if`,
`#ifdef`, `#ifndef`, `#elif`) with a trailing comment identifying the condition
being closed or branched (e.g., `#endif // CONFIG_VP9_HIGHBITDEPTH`, `#endif //
!CONFIG_VP9_HIGHBITDEPTH`, `#else // !CONFIG_FOO`, or `#endif // HAVE_NEON`).
Format trailing comments with two spaces before `//`. Short, trivial guards of
just a few lines where the opening condition is immediately obvious may omit
trailing comments.

**Why.** In long files or nested preprocessor guards (such as configuration
toggles, SIMD optimizations, or architecture guards), unannotated `#endif` and
`#else` directives make it difficult to determine which condition is ending or
branching without scrolling back. Trailing condition comments make block
boundaries immediately clear and prevent mismatched guards during refactoring
and maintenance.

## Examples

Bad:

```cpp
#if !CONFIG_VP9_HIGHBITDEPTH
void vpx_quantize_b_lsx(...) {
  // ... multi-line implementation ...
}
#endif

#ifdef ARCH_X86_64
void vpx_reset_mmx_state(void);
#else
void vpx_reset_mmx_state_c(void);
#endif

#if CONFIG_VP9
void vpx_yv12_copy_frame_c(const YV12_BUFFER_CONFIG *src_ybc,
                           YV12_BUFFER_CONFIG *dst_ybc) {
  // ... multi-line implementation ...
}
#endif
```

Good:

```cpp
#if !CONFIG_VP9_HIGHBITDEPTH
void vpx_quantize_b_lsx(...) {
  // ... multi-line implementation ...
}
#endif  // !CONFIG_VP9_HIGHBITDEPTH

#ifdef ARCH_X86_64
void vpx_reset_mmx_state(void);
#else   // !ARCH_X86_64
void vpx_reset_mmx_state_c(void);
#endif  // ARCH_X86_64

#if CONFIG_VP9
void vpx_yv12_copy_frame_c(const YV12_BUFFER_CONFIG *src_ybc,
                           YV12_BUFFER_CONFIG *dst_ybc) {
  // ... multi-line implementation ...
}
#endif  // CONFIG_VP9
```
