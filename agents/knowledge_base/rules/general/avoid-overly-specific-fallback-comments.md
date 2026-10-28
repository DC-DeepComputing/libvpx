---
name: libvpx/avoid-overly-specific-fallback-comments
description: When commenting defensive checks, fallback branches, or null guards, state the check's purpose plainly without speculating on overly specific or inaccurate trigger scenarios
rubric: readability
severity: should_fix
---

**Rule.** When adding comments to explain defensive checks, fallback logic, or
null-pointer guards, state the direct intent and action of the check plainly. Do
not include overly narrow, speculative, or inaccurate explanations of specific
triggering scenarios that may not account for all operating modes or codec
configurations.

**Why.** Overly specific or speculative comments mislead future maintainers
about subsystem invariants, failure modes, or valid operating configurations.
Plain explanations of what the fallback does remain accurate across evolving
code paths.

## Examples

Bad:

```cpp
// Force copy (no denoise, copy source in denoised buffer) if
// running_avg_y[frame] is NULL. This can happen in SVC if the top resolution
// is first-time encoded as inter-frame (i.e. no key frame on base for svc),
// because encoding started with lower layer only then enable top layer
// without key frame.
if (denoiser->running_avg_y[frame].buffer_alloc == NULL) {
  // ...
}
```

Good:

```cpp
// Force copy (no denoise, copy source in denoised buffer) if
// running_avg_y[frame] is NULL.
if (denoiser->running_avg_y[frame].buffer_alloc == NULL) {
  // ...
}
```
