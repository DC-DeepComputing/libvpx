---
name: libvpx/place-public-cpp-in-libvpx-namespace
description: When declaring public C++ interfaces in libvpx, place declarations in namespace libvpx and prefix types with the codec name instead of using codec-specific namespaces
rubric: readability
severity: nit
---

**Rule.** When declaring public or shared C++ types, interfaces, and helpers in
libvpx, enclose all declarations in top-level namespace libvpx rather than
defining codec-specific namespaces (such as namespace vp9 or namespace vp8).
Prefix codec-specific structs, classes, and types with their codec identifier
(e.g. VP9RateControlRTC, VP8RateControlRtcConfig) and shared base types with Vpx
(e.g. VpxRateControlRtcConfig) to prevent symbol collisions within the shared
namespace.

**Why.** Placing all C++ APIs within the top-level libvpx namespace provides a
unified, predictable namespace for downstream consumers (such as WebRTC and
Chromium) linking against libvpx. Explicit codec prefixes (VP8, VP9, Vpx) on
class and struct names prevent identifier collisions across different codecs
without introducing nested or fragmented codec namespaces.

## Examples

Bad:

```cpp
// Bad: codec-specific namespace and unprefixed type names
namespace vp9 {

struct RateControlRtcConfig {
  int width;
  int height;
  double framerate;
};

struct FrameParamsQpRTC {
  FRAME_TYPE frame_type;
  int spatial_layer_id;
};

class RateControlRTC { ... };

}  // namespace vp9
```

Good:

```cpp
// Good: top-level libvpx namespace with explicit codec-prefixed types
namespace libvpx {

struct VP9RateControlRtcConfig : public VpxRateControlRtcConfig {
  int width;
  int height;
  double framerate;
};

struct VP9FrameParamsQpRTC {
  FRAME_TYPE frame_type;
  int spatial_layer_id;
};

class VP9RateControlRTC { ... };

}  // namespace libvpx
```
