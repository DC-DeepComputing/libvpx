---
name: libvpx/clarify-collection-wide-function-names
description: When naming functions that iterate over a collection or pool to process all matching items, include _all_ or plural qualifiers rather than singular names
rubric: readability
severity: nit
---

**Rule.** When naming functions or helper routines that iterate over an entire
collection, pool, or array to process, reset, or release all matching items,
include `_all_` or a plural noun qualifier in the function name rather than a
singular item name. Reserve singular names for operations that act on an
individual item passed as a parameter or referenced by a specific index or
pointer.

**Why.** A singular name (such as `release_corrupted_fb`) falsely implies that
the function operates on a single item passed by the caller or referenced
directly. Adding `_all_` or plural qualifiers (such as
`release_all_corrupted_fb` or `release_frame_buffers`) immediately communicates
collection-wide sweep semantics to callers without requiring them to inspect the
function implementation.

## Examples

Bad:

```cpp
// Misleading singular name for a helper that loops over all entries in a pool
static INLINE void release_corrupted_fb(VP9_COMMON *cm) {
  int i;
  for (i = 0; i < FRAME_BUFFERS; ++i) {
    if (cm->buffer_pool->frame_bufs[i].buf.corrupted) {
      cm->buffer_pool->release_fb_cb(
          cm->buffer_pool->cb_priv,
          &cm->buffer_pool->frame_bufs[i].raw_frame_buffer);
    }
  }
}
```

Good:

```cpp
// Explicit '_all_' qualifier clearly conveys collection-wide iteration
static INLINE void release_all_corrupted_fb(VP9_COMMON *cm) {
  int i;
  for (i = 0; i < FRAME_BUFFERS; ++i) {
    if (cm->buffer_pool->frame_bufs[i].buf.corrupted) {
      cm->buffer_pool->release_fb_cb(
          cm->buffer_pool->cb_priv,
          &cm->buffer_pool->frame_bufs[i].raw_frame_buffer);
    }
  }
}
```
