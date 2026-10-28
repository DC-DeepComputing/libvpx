---
name: libvpx/vpx/keep-codec-specific-definitions-in-codec-headers
description: When defining codec-specific configuration structs, enums, or constants, place them in codec-specific headers like vp8cx.h rather than generic headers like vpx_encoder.h
rubric: design
severity: should_fix
---

**Rule.** Do not place codec-specific configuration structures, control enums,
or constants (such as VP8/VP9 SVC reference frame structures or codec buffer
limits) in codec-agnostic public headers like vpx/vpx_encoder.h,
vpx/vpx_decoder.h, or vpx/vpx_codec.h. Place codec-specific controls and data
structures in codec-specific public headers such as vpx/vp8cx.h, vpx/vp8dx.h, or
vpx/vp8.h.

**Why.** vpx/vpx_encoder.h, vpx/vpx_decoder.h, and vpx/vpx_codec.h define the
generic, codec-agnostic libvpx API shared across all codec implementations.
Placing codec-specific data structures or constants in generic headers pollutes
the public interface and introduces unnecessary coupling across codecs.

## Examples

Bad:

```cpp
// In vpx/vpx_encoder.h (generic encoder interface):
#define REF_FRAMES_LOG2 3
#define REF_FRAMES (1 << REF_FRAMES_LOG2)

typedef struct vpx_svc_ref_frame_config {
  int lst_fb_idx[VPX_SS_MAX_LAYERS];
  int gld_fb_idx[VPX_SS_MAX_LAYERS];
  int alt_fb_idx[VPX_SS_MAX_LAYERS];
} vpx_svc_ref_frame_config_t;
```

Good:

```cpp
// In vpx/vp8cx.h (VP8/VP9 encoder-specific controls):
typedef struct vpx_svc_ref_frame_config {
  int lst_fb_idx[VPX_SS_MAX_LAYERS];
  int gld_fb_idx[VPX_SS_MAX_LAYERS];
  int alt_fb_idx[VPX_SS_MAX_LAYERS];
} vpx_svc_ref_frame_config_t;
```
