---
name: libvpx/avoid-large-stack-allocations
rubric: safety
severity: should_fix
---

**Rule.** Do not declare large arrays or buffers (such as uncompressed video
frame buffers or multi-kilobyte temporary buffers) directly as local stack
variables. Use heap allocation (e.g., vpx_img_alloc(),
std::unique_ptr<uint8_t[]>, std::vector, or vpx_malloc()) in production code, or
test helpers like libvpx_test::RandomVideoSource and
libvpx_test::DummyVideoSource in unit tests, to avoid stack exhaustion.

**Why.** Video frame buffers and large pixel arrays frequently require megabytes
of storage. Allocating them on the stack risks stack overflow crashes,
particularly on worker threads with limited default stack sizes, embedded
targets, or when running under AddressSanitizer/MemorySanitizer where redzones
expand stack frames.

## Examples

Bad:

```cpp
cfg.g_w = 1540;
cfg.g_h = 838;
// Large frame buffer allocated directly on the stack risks stack overflow.
uint8_t img[1540 * 838 * 3 / 2];
vpx_image_t raw;
vpx_img_wrap(&raw, VPX_IMG_FMT_I420, cfg.g_w, cfg.g_h, 1, img);
```

Good:

```cpp
cfg.g_w = 1540;
cfg.g_h = 838;
// Use test video source helper or heap allocation for frame buffers.
libvpx_test::RandomVideoSource video;
video.SetSize(cfg.g_w, cfg.g_h);
video.SetImageFormat(VPX_IMG_FMT_I420);
```
