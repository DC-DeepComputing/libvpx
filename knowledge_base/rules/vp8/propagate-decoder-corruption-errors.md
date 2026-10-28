---
name: libvpx/vp8/propagate-decoder-corruption-errors
description: When encountering corrupted reference frames or decoding errors in decoder routines, record error status and propagate error info instead of silently returning early
rubric: correctness
severity: should_fix
---

**Rule.** When handling corrupted reference frames or decoding errors
(especially in multi-threaded decoding routines), do not silently return early
without updating error state. Record the corruption on the macroblock or worker
context and trigger vpx_internal_error() or set error tracking so the decoder
API reliably returns VPX_CODEC_CORRUPT_FRAME post-decode.

**Why.** Silently returning early from inner decoding loops prevents worker
crashes but swallows the error, causing the decoder to treat a corrupted frame
as successfully decoded and produce corrupt output without signaling an error.

## Examples

Bad:

```cpp
// In multi-threaded macroblock decoding: silently return early on corrupted reference
if (xd->corrupted) {
  return;
}
```

Good:

```cpp
// Record and trigger internal error info to ensure it is returned post-decode
if (xd->corrupted) {
  vpx_internal_error(&xd->error_info, VPX_CODEC_CORRUPT_FRAME,
                     "Corrupted reference frame");
}
```
