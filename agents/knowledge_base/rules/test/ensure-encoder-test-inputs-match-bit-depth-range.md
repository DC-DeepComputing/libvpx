---
name: libvpx/test/ensure-encoder-test-inputs-match-bit-depth-range
description: When supplying input frames in encoder tests, ensure pixel values strictly adhere to the configured bit depth range without testing out-of-contract inputs
rubric: testing
severity: should_fix
---

**Rule.** Ensure test video sources, raw YUV files, and synthetic input buffers
supplied to encoder tests contain pixel values strictly within the valid range
for the configured bit depth (e.g., [0, 1023] for 10-bit / VPX_BITS_10). Do not
add unit tests or encoder workarounds that feed out-of-range pixel values into
the encoder, as out-of-range input violates the encoder API contract.

**Why.** The libvpx encoder assumes valid input pixels within the configured bit
depth range as a precondition. Pixel values exceeding (1 << bit_depth) - 1
violate the API contract and cause internal calculation overflows or undefined
behavior that the encoder is not designed to handle or defend against.

## Examples

Bad:

```cpp
// Bad: Test file or buffer supplies pixel values >= 1024 for 10-bit encoding.
cfg.g_bit_depth = VPX_BITS_10;
cfg.g_input_bit_depth = 10;
libvpx_test::YUVVideoSource video("repro_pixel_overflow.yuv",
                                  VPX_IMG_FMT_I44416, 352, 288, 30, 1, 0, 10);
```

Good:

```cpp
// Good: Input pixels are strictly clamped or generated in [0, 1023].
cfg.g_bit_depth = VPX_BITS_10;
cfg.g_input_bit_depth = 10;
libvpx_test::YUVVideoSource video("valid_10bit_input.yuv",
                                  VPX_IMG_FMT_I44416, 352, 288, 30, 1, 0, 10);
```
