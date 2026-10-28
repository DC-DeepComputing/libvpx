---
name: libvpx/vp9/cache-encoder-state-before-destruction
description: When deallocating encoder instances or tearing down compressor contexts (cpi), cache required metrics and two-pass stats beforehand and avoid dereferencing freed handles.
rubric: safety
severity: blocking
---

**Rule.** Do not access encoder instances (cpi), wrapper implementation handles,
or their internal sub-structures (such as twopass or rc) after deallocating them
via free_encoder(), vp9_remove_compressor(), or vpx_codec_destroy(). Extract,
compute, and cache any required frame counts, pass statistics, or configuration
state into persistent members before destroying the encoder, and nullify freed
pointers.

**Why.** Calling free_encoder() or vp9_remove_compressor() frees the underlying
compressor struct and all associated sub-allocations. Accessing fields of the
destroyed encoder causes use-after-free bugs and undefined behavior across
subsequent encoding passes or query calls.

## Examples

Bad:

```cpp
void SimpleEncode::ComputeFrameStats() {
  free_encoder(impl_ptr_->cpi);
  // BAD: Accessing twopass state on cpi after deallocating the encoder.
  impl_ptr_->num_coding_frames =
      vp9_get_coding_frame_num(&oxcf, &impl_ptr_->cpi->twopass, ...);
}
```

Good:

```cpp
void SimpleEncode::ComputeFrameStats() {
  // GOOD: Cache required frame counts and stats before destroying the encoder.
  impl_ptr_->num_coding_frames =
      vp9_get_coding_frame_num(&oxcf, &impl_ptr_->cpi->twopass, ...);
  free_encoder(impl_ptr_->cpi);
  impl_ptr_->cpi = nullptr;
}
```
