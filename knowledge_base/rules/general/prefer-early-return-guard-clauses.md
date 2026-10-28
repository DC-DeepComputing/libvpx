---
name: libvpx/prefer-early-return-guard-clauses
description: When checking preconditions, resource allocations, or error conditions, prefer early return guard clauses over wrapping subsequent function logic in nested if blocks
rubric: readability
severity: nit
---

**Rule.** Use early return guard clauses to handle failed preconditions,
allocation failures, or invalid states instead of wrapping the remainder of the
function body in an if block. Keep the primary execution path at the outer
indentation level rather than nested inside conditional success blocks.

**Why.** Returning immediately upon encountering an error or invalid condition
flattens nested control flow, keeps the happy path at a consistent shallow
indentation level, and reduces cognitive load by handling failure cases upfront.

## Examples

Bad:

```cpp
std::unique_ptr<VP9RateControlRTC> rc_api(
    new (std::nothrow) VP9RateControlRTC());
if (rc_api) {
  rc_api->cpi_ = static_cast<VP9_COMP *>(vpx_memalign(32, sizeof(*cpi_)));
  if (rc_api->cpi_ == nullptr) {
    return nullptr;
  }
  rc_api->InitRateControl(cfg);
}
return rc_api;
```

Good:

```cpp
std::unique_ptr<VP9RateControlRTC> rc_api(
    new (std::nothrow) VP9RateControlRTC());
if (!rc_api) return nullptr;

rc_api->cpi_ = static_cast<VP9_COMP *>(vpx_memalign(32, sizeof(*cpi_)));
if (rc_api->cpi_ == nullptr) {
  return nullptr;
}
rc_api->InitRateControl(cfg);
return rc_api;
```
