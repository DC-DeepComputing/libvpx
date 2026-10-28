---
name: libvpx/use-abort-instead-of-assert-for-fatal-conditions
description: When handling impossible conditions or fatal errors that must terminate in optimized builds, use abort() rather than assert(0) or assert(false)
rubric: correctness
severity: should_fix
---

**Rule.** Use abort() rather than assert(0), assert(false), or assert(!"...")
for impossible conditions, unreachable branches, and fatal error paths that must
unconditionally terminate execution. In optimized and release builds where
NDEBUG is defined, assert() is compiled out, allowing execution to continue into
invalid states or undefined behavior.

**Why.** assert() is eliminated by the preprocessor when NDEBUG is defined in
release and optimized test builds. Calling abort() guarantees unconditional
process termination across all build configurations and compiler optimization
levels.

## Examples

Bad:

```cpp
if (encode_frame_result->coding_data_byte_size <= 0) {
  fprintf(stderr, "Coding data size <= 0\n");
  assert(0);  // Disabled when NDEBUG is defined in release/optimized builds!
}
```

Good:

```cpp
if (encode_frame_result->coding_data_byte_size <= 0) {
  fprintf(stderr, "Coding data size <= 0\n");
  abort();  // Unconditionally terminates across all build configurations.
}
```
