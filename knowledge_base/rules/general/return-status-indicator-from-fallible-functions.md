---
name: libvpx/return-status-indicator-from-fallible-functions
description: When defining functions or configuration methods that perform input validation or fallible operations, return a status code or boolean instead of void
rubric: correctness
severity: should_fix
---

**Rule.** When declaring or implementing functions, methods, or setters that
perform input validation, parse configuration, or execute operations that can
fail, return an explicit status indicator (such as a status enum or boolean)
rather than returning void. Ensure callers can inspect the return value to
detect and handle invalid parameters or runtime failures.

**Why.** Returning void from fallible operations or configuration setters
silently discards error conditions, leaving callers unable to distinguish
between success and failure. Propagating a status code or boolean return value
allows callers to handle invalid inputs or failures safely.

## Examples

Bad:

```cpp
// Silent failure: caller cannot detect invalid configuration parameters.
void SetEncodeConfig(const char *name, const char *value) {
  if (!IsValidConfig(name, value)) return;
  ApplyConfig(name, value);
}
```

Good:

```cpp
// Explicit status return enables callers to detect and handle errors.
StatusCode SetEncodeConfig(const char *name, const char *value) {
  if (!IsValidConfig(name, value)) return STATUS_INVALID_PARAM;
  ApplyConfig(name, value);
  return STATUS_SUCCESS;
}
```
