---
name: libvpx/vp9/avoid-out-of-band-header-decryption
description: When parsing frame headers or handling decoder resync, parse headers within the sequential decode pipeline instead of decrypting out-of-band with vpx_decrypt_cb
rubric: correctness
severity: should_fix
---

**Rule.** Do not speculatively parse or decrypt frame headers out-of-band prior
to the main sequential decoding pipeline. Execute header-dependent actions (such
as keyframe detection or buffer resynchronization) inside the primary header
parser (read_uncompressed_header) where decryption (vpx_decrypt_cb) is sequenced
properly.

**Why.** Decryption callbacks (vpx_decrypt_cb) rely on stateful context
(decrypt_state), such as stream cipher counters or sub-sample offsets. Invoking
decryption out-of-band to inspect headers mutates or desynchronizes decryptor
state, corrupting subsequent frame payload decryption.

## Examples

Bad:

```cpp
// In vp9_receive_compressed_data():
if (pbi->need_resync) flush_all_fb_on_key(pbi, source, source + size);

static INLINE void flush_all_fb_on_key(VP9Decoder *pbi, const uint8_t *data,
                                       const uint8_t *data_end) {
  uint8_t clear[MAX_VP9_HEADER_SIZE];
  struct vpx_read_bit_buffer rb;
  // Decrypts out-of-band and desynchronizes decrypt_state:
  init_read_bit_buffer(pbi, &rb, data, data_end, clear);
}
```

Good:

```cpp
// In read_uncompressed_header() in vp9_decodeframe.c:
if (pbi->need_resync) {
  memset(&cm->ref_frame_map, -1, sizeof(cm->ref_frame_map));
  flush_all_fb_on_key(cm);
  pbi->need_resync = 0;
}
```
