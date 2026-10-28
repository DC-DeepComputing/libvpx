---
name: libvpx/use-nothrow-new-for-heap-allocation
description: When allocating C++ heap memory, include <new>, use new (std::nothrow) instead of bare new, and check the pointer for nullptr before dereferencing
rubric: safety
severity: should_fix
---

**Rule.** When allocating C++ heap memory or creating factory objects, include
`<new>` and use `new (std::nothrow)` rather than throwing bare `new`.
Additionally, always check that the resulting pointer or smart pointer
(`std::unique_ptr`) is non-null before dereferencing it or accessing members,
handling allocation failure by returning `nullptr`, propagating an error code,
or performing necessary cleanup.

**Why.** libvpx avoids C++ exceptions and relies on `nullptr` return checks to
handle allocation failures gracefully. Bare `new` throws `std::bad_alloc` on
allocation failure, rendering null checks ineffective or leading to unhandled
exceptions. Conversely, `new (std::nothrow)` returns `nullptr` on failure, so
dereferencing or accessing members without checking for `nullptr` causes
null-pointer dereference crashes.

## Examples

Bad:

```cpp
#include <memory>

// Variant 1: Bare new used without std::nothrow
std::unique_ptr<VP9RateControlRTC> VP9RateControlRTC::Create(
    const VP9RateControlRtcConfig &cfg) {
  std::unique_ptr<VP9RateControlRTC> rc_api(new VP9RateControlRTC());
  if (!rc_api) return nullptr;
  return rc_api;
}

// Variant 2: Missing nullptr check after nothrow allocation
std::unique_ptr<VP9RateControlRTC> rc_api(
    new (std::nothrow) VP9RateControlRTC());
rc_api->cpi_ = static_cast<VP9_COMP *>(vpx_memalign(32, sizeof(*cpi_)));
```

Good:

```cpp
#include <new>
#include <memory>

// Variant 1: Using new (std::nothrow) with nullptr check
std::unique_ptr<VP9RateControlRTC> VP9RateControlRTC::Create(
    const VP9RateControlRtcConfig &cfg) {
  std::unique_ptr<VP9RateControlRTC> rc_api(
      new (std::nothrow) VP9RateControlRTC());
  if (!rc_api) return nullptr;
  if (!rc_api->InitRateControl(cfg)) return nullptr;
  return rc_api;
}

// Variant 2: Verifying pointer before member access
std::unique_ptr<VP9RateControlRTC> rc_api(
    new (std::nothrow) VP9RateControlRTC());
if (!rc_api) return nullptr;
rc_api->cpi_ = static_cast<VP9_COMP *>(vpx_memalign(32, sizeof(*cpi_)));
```
