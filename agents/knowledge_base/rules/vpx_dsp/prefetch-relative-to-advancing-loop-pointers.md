---
name: libvpx/vpx_dsp/prefetch-relative-to-advancing-loop-pointers
rubric: safety
severity: should_fix
---

**Rule.** When issuing software prefetch instructions (such as
__builtin_prefetch) inside loops over multi-row or multi-block buffers, compute
prefetch target addresses relative to actively advancing cursor pointers rather
than stationary outer base pointers. Ensure the offset projects ahead of the
current loop iteration to prefetch upcoming cache lines.

**Why.** Prefetching relative to fixed base pointers that do not advance across
loop iterations repeatedly fetches the start of the buffer on every iteration,
wasting prefetch bandwidth and failing to bring upcoming cache lines into memory
ahead of the execution cursor.

## Examples

Bad:

```cpp
void process_block(const uint8_t *src, int src_stride, uint8_t *dst,
                   int dst_stride, int h) {
  const uint8_t *s = src;
  uint8_t *d = dst;
  for (int y = 0; y < h; ++y) {
    // Bad: stationary base pointers repeatedly prefetch the start of the buffer
    __builtin_prefetch(src + 2 * src_stride);
    __builtin_prefetch(dst + 2 * dst_stride);
    process_row(s, d);
    s += src_stride;
    d += dst_stride;
  }
}
```

Good:

```cpp
void process_block(const uint8_t *src, int src_stride, uint8_t *dst,
                   int dst_stride, int h) {
  const uint8_t *s = src;
  uint8_t *d = dst;
  for (int y = 0; y < h; ++y) {
    // Good: calculates prefetch address relative to actively advancing pointers
    __builtin_prefetch(s + 2 * src_stride);
    __builtin_prefetch(d + 2 * dst_stride);
    process_row(s, d);
    s += src_stride;
    d += dst_stride;
  }
}
```
