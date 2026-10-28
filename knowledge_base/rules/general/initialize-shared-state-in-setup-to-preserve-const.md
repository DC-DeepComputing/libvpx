---
name: libvpx/initialize-shared-state-in-setup-to-preserve-const
description: When implementing C++ query or inspection methods, initialize shared state in setup routines rather than mutating it inside const methods.
rubric: readability
severity: should_fix
---

**Rule.** Initialize and populate shared state structures during setup,
initialization, or pass-generation routines rather than mutating them inside
query or calculation functions. Ensure query and calculation methods remain
side-effect free so that internal handles and context pointers (such as const
VP9_COMP *) remain strictly const-qualified.

**Why.** Mutating shared internal state within query or calculation methods
violates logical const correctness, introduces unexpected side effects into
read-only helpers, and prevents context pointers from being marked const.

## Examples

Bad:

```cpp
std::vector<int> SimpleEncode::ComputeKeyFrameMap() const {
  VP9_COMP *cpi = impl_ptr_->cpi;
  fps_init_first_pass_info(&cpi->twopass.first_pass_info,
                           GetVectorData(impl_ptr_->first_pass_stats),
                           num_frames_);
  std::vector<int> key_frame_map(num_frames_, 0);
  vp9_get_key_frame_map(&oxcf, &cpi->twopass, GetVectorData(key_frame_map));
  return key_frame_map;
}
```

Good:

```cpp
// In ComputeFirstPassStats():
fps_init_first_pass_info(&impl_ptr_->cpi->twopass.first_pass_info,
                         GetVectorData(impl_ptr_->first_pass_stats),
                         num_frames_);

// In ComputeKeyFrameMap() const:
const VP9_COMP *cpi = impl_ptr_->cpi;
std::vector<int> key_frame_map(num_frames_, 0);
vp9_get_key_frame_map(&oxcf, &cpi->twopass, GetVectorData(key_frame_map));
return key_frame_map;
```
