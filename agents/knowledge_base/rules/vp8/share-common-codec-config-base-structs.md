---
name: libvpx/vp8/share-common-codec-config-base-structs
description: When defining configuration or rate control structs across VP8 and VP9, factor shared fields into base structs in vpx/internal/ rather than duplicating them
rubric: design
severity: should_fix
---

**Rule.** When introducing or modifying configuration or rate control structures
across individual codec implementations (such as vp8/ and vp9/), factor common,
codec-agnostic parameters into a shared base struct in an internal header (such
as under vpx/internal/). Codec-specific configuration structs should inherit
from or compose the shared base struct rather than duplicating member fields and
defaults.

**Why.** Duplicating common configuration parameters across separate codec
directories increases maintenance overhead, makes API updates error-prone, and
risks subtle divergence in default values or parameter semantics between codecs.

## Examples

Bad:

```cpp
// In vp8/vp8_ratectrl_rtc.h - duplicating common rate control fields from VP9
struct VP8RateControlRtcConfig {
  int width;
  int height;
  int max_quantizer;
  int min_quantizer;
  int64_t target_bandwidth;
  // VP8-specific fields...
};
```

Good:

```cpp
// In vpx/internal/vpx_ratectrl_rtc.h - shared base struct
struct VpxRateControlRtcConfig {
  int width;
  int height;
  int max_quantizer;
  int min_quantizer;
  int64_t target_bandwidth;
};

// In vp8/vp8_ratectrl_rtc.h - codec-specific struct extending shared base
struct VP8RateControlRtcConfig : public VpxRateControlRtcConfig {
  // VP8-specific fields and initialization...
};
```
