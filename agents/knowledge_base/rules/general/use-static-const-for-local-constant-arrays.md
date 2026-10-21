---
name: libvpx/use-static-const-for-local-constant-arrays
rubric: safety
severity: should_fix
---

**Rule.** Declare local constant arrays, lookup tables, and SIMD constant
vectors inside functions as `static const` rather than automatic `const` or
non-static arrays. In C, automatic local arrays with initializers incur
per-invocation stack allocation and copy overhead, whereas `static const` places
the table in read-only data storage.

**Why.** Non-static local arrays with initializers force the compiler to
allocate stack space and copy initial element values on every function call.
Marking them `static const` ensures they reside in `.rodata` with static
lifetime, eliminating unnecessary runtime stack allocation and per-call
initialization overhead.

## Examples

Bad:

```cpp
// Bad: Automatic array is re-initialized on the stack on every call.
void diamond_search(void) {
  const uint32_t data[4] = { 0, 1, 2, 3 };
  const int8_t cost_table[8] = { 0, 1, 1, 2, 2, 3, 3, 4 };
  // ...
}
```

Good:

```cpp
// Good: Stored in read-only data (.rodata); no per-call stack initialization.
void diamond_search(void) {
  static const uint32_t data[4] = { 0, 1, 2, 3 };
  static const int8_t cost_table[8] = { 0, 1, 1, 2, 2, 3, 3, 4 };
  // ...
}
```
