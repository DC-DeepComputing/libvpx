---
name: libvpx/test/override-fillframe-in-dummyvideosource-subclasses
description: When customizing test frame pixel data in DummyVideoSource subclasses, override the virtual FillFrame() method instead of placing image generation in Next().
rubric: design
severity: should_fix
---

**Rule.** When generating custom frame content or manipulating pixel buffers in
DummyVideoSource test subclasses, override the virtual FillFrame() method
instead of placing image generation logic inside Next() or Begin(). Keep Next()
and Begin() focused on frame iteration and dimension updates (SetSize()),
allowing the base class workflow to invoke FillFrame() to populate image
buffers.

**Why.** DummyVideoSource delegates frame buffer population to the virtual
FillFrame() hook from both Begin() and Next(). Overriding FillFrame() ensures
custom pixel generation executes consistently across all iteration phases
without duplicating logic or conflating iteration state with image data
generation.

## Examples

Bad:

```cpp
class ResizingVideoSource : public libvpx_test::DummyVideoSource {
 public:
  void Next() override {
    ++frame_;
    SetSize(width, height);
    FillFrame();
    if (random_input_) {
      libvpx_test::ACMRandom rnd(libvpx_test::ACMRandom::DeterministicSeed());
      unsigned char *image = img_->planes[0];
      for (size_t i = 0; i < raw_sz_; ++i) {
        image[i] = rnd.Rand8();
      }
    }
  }
};
```

Good:

```cpp
class ResizingVideoSource : public libvpx_test::DummyVideoSource {
 public:
  void Next() override {
    ++frame_;
    SetSize(width, height);
    FillFrame();
  }

  void FillFrame() override {
    if (img_) memset(img_->img_data, 0, raw_sz_);
    if (random_input_) {
      libvpx_test::ACMRandom rnd(libvpx_test::ACMRandom::DeterministicSeed());
      unsigned char *image = img_->planes[0];
      for (size_t i = 0; i < raw_sz_; ++i) {
        image[i] = rnd.Rand8();
      }
    }
  }
};
```
