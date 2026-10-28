---
name: libvpx/canonical-codec-mode-terminology-in-comments
description: When commenting on codec configuration modes or tuning settings, use canonical API terminology (e.g., 'screen content mode') rather than application-specific colloquialisms like 'screenshare'.
rubric: readability
severity: nit
---

**Rule.** In code comments and documentation, refer to codec configuration
modes, tuning parameters, and content types by their canonical API and
specification terminology (such as 'screen content mode' for VP9E_CONTENT_SCREEN
or VP8E_SET_SCREEN_CONTENT_MODE). Do not use application-specific colloquialisms
(such as 'screenshare') that mischaracterize general codec features as
application-exclusive.

**Why.** Codec tuning flags and content modes apply broadly to media types (such
as screen capture content in animations, slides, or desktop rendering), not
exclusively to specific applications like screensharing. Using canonical API
terminology keeps comments precise, consistent, and accurate across the
codebase.

## Examples

Bad:

```cpp
// Enable post encode frame dropping for screenshare.
if (cpi->oxcf.content == VP9E_CONTENT_SCREEN && cpi->oxcf.rc_mode == VPX_CBR) {
  cpi->drop_frames_water_mark = 30;
}
```

Good:

```cpp
// Enable post encode frame dropping for screen content mode.
if (cpi->oxcf.content == VP9E_CONTENT_SCREEN && cpi->oxcf.rc_mode == VPX_CBR) {
  cpi->drop_frames_water_mark = 30;
}
```
