---
name: libvpx/consistent-pointer-null-checks
description: When checking pointer validity or nullness, maintain a consistent style across the file or class without mixing implicit boolean tests and explicit NULL comparisons.
rubric: readability
severity: nit
---

**Rule.** Maintain consistency in pointer null checks across a file, class, or
module. Avoid mixing implicit boolean tests (e.g., `if (ptr)` or `if (!ptr)`)
with explicit null comparisons (e.g., `if (ptr != NULL)` or `if (ptr == NULL)`)
within the same scope, adopting the established style of the surrounding file.

**Why.** Mixing implicit boolean pointer tests and explicit null comparisons
within the same file or class causes stylistic inconsistency and degrades code
readability and maintainability.

## Examples

Bad:

```cpp
// Inconsistent: mixing explicit NULL check with implicit boolean test
if (out_file_ != NULL) {
  ivf_write_file_header_simple(out_file_, ...);
}
if (in_file_) {
  read_input_frame(in_file_, ...);
}
```

Good:

```cpp
// Consistent: using uniform explicit null checks matching the file style
if (out_file_ != NULL) {
  ivf_write_file_header_simple(out_file_, ...);
}
if (in_file_ != NULL) {
  read_input_frame(in_file_, ...);
}
```
