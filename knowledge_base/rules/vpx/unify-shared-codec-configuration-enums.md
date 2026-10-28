---
name: libvpx/vpx/unify-shared-codec-configuration-enums
description: When defining configuration options or codec controls shared across VP8 and VP9, reuse a single shared enum rather than duplicating codec-specific enums
rubric: design
severity: should_fix
---

**Rule.** When an enum or configuration option represents identical semantics
across multiple codecs (such as VP8 and VP9), reuse or define a single shared
enum across both codec interfaces. Do not define duplicate codec-specific enums
(such as parallel vp8e_* and vp9e_* types) and write translation or mapping
boilerplate between them.

**Why.** Duplicating enum definitions across codec subsystems introduces
unnecessary type fragmentation and requires repetitive translation logic in
codec control handlers when the options share identical semantics.

## Examples

Bad:

```cpp
// Duplicated enum definitions and translation boilerplate:
typedef enum { VP8_TUNE_PSNR, VP8_TUNE_SSIM } vp8e_tuning;
typedef enum { VP9_TUNE_PSNR, VP9_TUNE_SSIM } vp9e_tuning;

// In vp9_cx_iface.c:
const vp8e_tuning tuning = CAST(VP8E_SET_TUNING, args);
extra_cfg.tuning = (tuning == VP8_TUNE_SSIM) ? VP9_TUNE_SSIM : VP9_TUNE_PSNR;
```

Good:

```cpp
// Shared enum reused directly across both codecs:
typedef enum { VP8_TUNE_PSNR, VP8_TUNE_SSIM } vp8e_tuning;

// In vp9_cx_iface.c:
extra_cfg.tuning = CAST(VP8E_SET_TUNING, args);
```
