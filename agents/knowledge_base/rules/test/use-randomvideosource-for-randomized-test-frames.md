---
name: libvpx/test/use-randomvideosource-for-randomized-test-frames
description: When generating synthetic or randomized video frames in tests, use libvpx_test::RandomVideoSource instead of manually allocating buffers and filling bytes with ACMRandom
rubric: readability
severity: should_fix
---

**Rule.** In test cases requiring synthetic or randomized video input frames,
use test harness helpers such as `libvpx_test::RandomVideoSource` (or
`libvpx_test::DummyVideoSource`) rather than manually allocating memory buffers,
populating raw bytes with `ACMRandom`, and wrapping them with `vpx_img_wrap`.
Configure frame dimensions via `SetSize()`, format via `SetImageFormat()`, and
frame limits via `set_limit()`.

**Why.** Using `RandomVideoSource` eliminates repetitive boilerplate for memory
allocation, image wrapping, and pixel randomization while standardizing frame
iteration, PTS, and duration handling across test suites.

## Examples

Bad:

```cpp
uint8_t img[1540 * 838 * 3 / 2];
libvpx_test::ACMRandom rng;
vpx_image_t img_wrapper;
for (int num_frames = 0; num_frames < 10; num_frames++) {
  for (size_t i = 0; i < sizeof(img); ++i) {
    img[i] = rng.Rand8();
  }
  ASSERT_EQ(
      vpx_img_wrap(&img_wrapper, VPX_IMG_FMT_I420, cfg.g_w, cfg.g_h, 1, img),
      &img_wrapper);
  ASSERT_EQ(vpx_codec_encode(&enc, &img_wrapper, num_frames, 300, 0,
                             VPX_DL_GOOD_QUALITY),
            VPX_CODEC_OK);
}
```

Good:

```cpp
libvpx_test::RandomVideoSource video;
video.SetSize(cfg.g_w, cfg.g_h);
video.SetImageFormat(VPX_IMG_FMT_I420);
video.set_limit(10);
video.Begin();
do {
  ASSERT_EQ(vpx_codec_encode(&enc, video.img(), video.pts(), video.duration(),
                             0, VPX_DL_GOOD_QUALITY),
            VPX_CODEC_OK);
  video.Next();
} while (video.img() != nullptr);
```
