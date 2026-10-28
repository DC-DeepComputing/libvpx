---
name: libvpx/initialize-struct-fields-in-default-constructor
description: When defining default constructors for configuration or context structs, initialize all member fields and array elements to valid defaults or zero them
rubric: correctness
severity: should_fix
---

**Rule.** When defining default constructors for configuration structs, context
objects, or parameter structs, ensure all member fields—especially multi-layer
array members—are explicitly initialized to valid defaults, zero-initialized
in-class, or cleared with vp9_zero()/memset. Do not leave array members or
secondary layer settings uninitialized when establishing default configurations.

**Why.** Omitting array members or layer-specific settings from constructor
initialization leaves them containing indeterminate garbage values. Callers
relying on a default-constructed configuration will experience undefined
behavior, uninitialized memory reads, or subtle encoding failures.

## Examples

Bad:

```cpp
VP9RateControlRtcConfig() {
  width = 1280;
  height = 720;
  ss_number_layers = ts_number_layers = 1;
  // max_quantizers, min_quantizers, layer_target_bitrate left uninitialized
}
```

Good:

```cpp
VP9RateControlRtcConfig() {
  width = 1280;
  height = 720;
  ss_number_layers = ts_number_layers = 1;
  vp9_zero(max_quantizers);
  vp9_zero(min_quantizers);
  vp9_zero(layer_target_bitrate);
  layer_target_bitrate[0] = target_bandwidth;
  max_quantizers[0] = max_quantizer;
  min_quantizers[0] = min_quantizer;
}
```
