---
name: libvpx/explicitly-cast-void-pointers-to-typed-pointers
description: When unpacking, assigning, or converting generic `void *` or `const void *` arguments, callback parameters, or buffers to concrete typed pointers, use an explicit cast
rubric: readability
severity: should_fix
---

**Rule.** When unpacking, assigning, or converting generic `void *` or `const
void *` pointers (such as worker or hook callback parameters, user data, opaque
context pointers, or raw memory buffers) to concrete typed pointers (such as
struct pointers like `LFWorkerData *`, `VP9LfSync *`, or primitive pointers like
`uint8_t *`, `uint16_t *`), explicitly cast them using a C-style cast `(Type *)`
in C files or `static_cast<Type *>` in C++ files. Do not rely on C's implicit
pointer conversions from `void *`.

**Why.** In C, implicit conversions from `void *` to typed pointers are
syntactically valid but obscure the expected concrete type at conversion
boundaries such as worker thread hooks, generic callbacks, and buffer processing
functions. Explicit casting makes type assumptions visible and unambiguous, aids
code navigation and static analysis, prevents accidental type mismatches across
refactorings, and maintains compatibility with C++ compilers and strict
toolchain configurations.

## Examples

Bad:

```cpp
// Worker callback unpacking:
static int loop_filter_worker(void *arg1, void *arg2) {
  VP9LfSync *const lf_sync = arg1;
  LFWorkerData *const lf_data = arg2;
  // ...
}

// Buffer processing:
void process_buffer(void *buf, const void *src) {
  uint8_t *const dst = buf;
  const uint16_t *const input = src;
}
```

Good:

```cpp
// Worker callback unpacking:
static int loop_filter_worker(void *arg1, void *arg2) {
  VP9LfSync *const lf_sync = (VP9LfSync *)arg1;
  LFWorkerData *const lf_data = (LFWorkerData *)arg2;
  // ...
}

// Buffer processing:
void process_buffer(void *buf, const void *src) {
  uint8_t *const dst = (uint8_t *)buf;
  const uint16_t *const input = (const uint16_t *)src;
}
```
