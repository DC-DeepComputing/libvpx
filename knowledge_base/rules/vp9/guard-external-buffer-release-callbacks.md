---
name: libvpx/vp9/guard-external-buffer-release-callbacks
description: When releasing external frame buffers or tearing down decoder buffer pools, verify the dedicated released flag rather than internal buffer pointers to prevent duplicate releases
rubric: readability
severity: should_fix
---

**Rule.** When releasing external frame buffers (via release_fb_cb or
release_ext_fb_cb) during frame decoding or buffer pool teardown, track and
verify the buffer's release state using the dedicated released flag
(!frame_buf->released) rather than checking raw_frame_buffer.data or .priv
pointers. Set the released flag to 1 immediately upon invoking the release
callback (and reset to 0 upon allocation) to guard against duplicate callback
invocations.

**Why.** External buffer managers track buffer lifecycles and expect release
callbacks exactly once per allocation; duplicate invocations cause double-free
errors and allocator corruption. Relying directly on the boolean released flag
ensures consistent state tracking across buffer pool teardown and decode error
paths without checking internal raw buffer payload pointers.

## Examples

Bad:

```cpp
// Missing released flag check or redundantly checking internal buffer pointers
if (pool->frame_bufs[i].raw_frame_buffer.data != NULL) {
  pool->release_fb_cb(pool->cb_priv, &pool->frame_bufs[i].raw_frame_buffer);
  pool->frame_bufs[i].ref_count = 0;
}

if (!frame_bufs[i].released && frame_bufs[i].raw_frame_buffer.priv) {
  pool->release_fb_cb(pool->cb_priv, &frame_bufs[i].raw_frame_buffer);
  frame_bufs[i].released = 1;
}
```

Good:

```cpp
if (!pool->frame_bufs[i].released) {
  pool->release_fb_cb(pool->cb_priv, &pool->frame_bufs[i].raw_frame_buffer);
  pool->frame_bufs[i].ref_count = 0;
  pool->frame_bufs[i].released = 1;
}
```
