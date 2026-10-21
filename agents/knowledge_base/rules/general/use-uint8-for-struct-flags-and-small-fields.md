---
name: libvpx/use-uint8-for-struct-flags-and-small-fields
rubric: safety
severity: should_fix
---

**Rule.** When defining struct fields or array members that store boolean flags,
small enumerations, or small-range numeric values, use compact fixed-width
integer types such as uint8_t (or uint16_t) instead of full-width unsigned int
or int. This prevents unnecessary memory overhead and keeps data structure
layouts compact, particularly in packet headers, configuration structs, and
repeated array elements.

**Why.** Using 32-bit unsigned int or int for boolean flags or small integer
fields inflates struct size and increases cache and memory footprints, which is
especially detrimental in fixed-size packet headers (like vpx_codec_cx_pkt),
frame descriptors, or frequently allocated codec data structures.

## Examples

Bad:

```cpp
struct {
  /* ... */
  unsigned int spatial_layer_encoded[VPX_SS_MAX_LAYERS];
  unsigned int is_key_frame;
} frame;
```

Good:

```cpp
struct {
  /* ... */
  uint8_t spatial_layer_encoded[VPX_SS_MAX_LAYERS];
  uint8_t is_key_frame;
} frame;
```
