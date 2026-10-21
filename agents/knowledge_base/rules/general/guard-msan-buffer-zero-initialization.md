---
name: libvpx/guard-msan-buffer-zero-initialization
rubric: safety
severity: should_fix
---

**Rule.** When zero-initializing dynamically allocated memory buffers (such as
frame buffers) purely to silence MemorySanitizer (MSan) uninitialized-read
warnings rather than for functional codec correctness, guard the memset() call
under `#if defined(__has_feature)` and `#if __has_feature(memory_sanitizer)`
checks. Do not execute unconditional memset operations on large allocations in
production code.

**Why.** Unconditionally zero-initializing large frame or pixel buffers
introduces substantial runtime CPU overhead and cache churn in production
encoding and decoding loops. Guarding sanitizer-only cleanups behind MSan
preprocessor checks keeps production builds fast while keeping MSan tests clean.

## Examples

Bad:

```cpp
ybf->buffer_alloc = (uint8_t *)vpx_memalign(32, frame_size);
if (!ybf->buffer_alloc) return -1;
// Silencing MSan warnings unconditionally hurts production performance.
memset(ybf->buffer_alloc, 0, frame_size);
```

Good:

```cpp
ybf->buffer_alloc = (uint8_t *)vpx_memalign(32, frame_size);
if (!ybf->buffer_alloc) return -1;
#if defined(__has_feature)
#if __has_feature(memory_sanitizer)
memset(ybf->buffer_alloc, 0, frame_size);
#endif
#endif
```
