---
name: libvpx/do-not-release-unique-ptr-on-error
description: When handling error exits or returning nullptr from functions managing local std::unique_ptr instances, do not call .release(); rely on RAII for automatic cleanup.
rubric: safety
severity: should_fix
---

**Rule.** Do not call `.release()` on a local `std::unique_ptr` when handling
error exits, initialization failures, or returning `nullptr` or error codes.
Rely on RAII to automatically destruct and free the managed resource when the
`std::unique_ptr` goes out of scope.

**Why.** Calling `.release()` relinquishes ownership and returns the raw pointer
without destroying the managed object, causing a memory leak on early exit or
error return paths. RAII guarantees automatic cleanup when the smart pointer
falls out of scope.

## Examples

Bad:

```cpp
std::unique_ptr<VP9RateControlRTC> rc_api(new (std::nothrow) VP9RateControlRTC());
if (!rc_api) return nullptr;
rc_api->cpi_ = static_cast<VP9_COMP *>(vpx_memalign(32, sizeof(*cpi_)));
if (!rc_api->cpi_) {
  rc_api.release();  // Leaks rc_api!
  return nullptr;
}
```

Good:

```cpp
std::unique_ptr<VP9RateControlRTC> rc_api(new (std::nothrow) VP9RateControlRTC());
if (!rc_api) return nullptr;
rc_api->cpi_ = static_cast<VP9_COMP *>(vpx_memalign(32, sizeof(*cpi_)));
if (!rc_api->cpi_) {
  // Let rc_api go out of scope to automatically destruct and free memory via RAII.
  return nullptr;
}
```
