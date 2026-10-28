---
name: libvpx/vp9/distinguish-superframe-from-key-frame-terminology
description: When documenting VP9 SVC superframes or spatial layer keying, specify that spatial layers or superframes have a key base layer rather than calling superframes key frames
rubric: readability
severity: nit
---

**Rule.** In VP9 SVC code comments and struct documentation, use precise
terminology when describing superframes and spatial layer frame types. Refer to
superframes whose base layer is key (or note that all spatial layers in a
superframe are encoded as key frames) rather than ambiguously calling the
multi-layer superframe itself a 'key frame'.

**Why.** In VP9 SVC, a superframe is a multi-layer container bundling individual
spatial layer frames, each having its own header and frame type. Referring to
the entire superframe as a 'key frame' is inaccurate and obscures whether
individual spatial layers use inter-layer or intra-frame prediction.

## Examples

Bad:

```cpp
// No inter-layer prediction. Every spatial layer on a key frame is key.
int simulcast_mode;
```

Good:

```cpp
// Every spatial layer on a superframe whose base is key is key too.
int simulcast_mode;
```
