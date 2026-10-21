---
name: libvpx/scope-sanitizer-suppressions-to-minimal-helpers
rubric: safety
severity: should_fix
---

**Rule.** When suppressing compiler sanitizer checks (such as using
VPX_NO_UNSIGNED_OVERFLOW_CHECK or VPX_NO_UNSIGNED_SHIFT_CHECK), isolate the
attribute on the smallest possible static helper function containing only the
specific arithmetic or shift operation where overflow is intentional. Do not
annotate entire outer functions or multi-statement algorithms with sanitizer
suppression attributes.

**Why.** Annotating an entire function disables runtime sanitizer checking (such
as UBSan unsigned integer overflow or shift checks) for all operations across
the function body, masking unintended bugs and arithmetic regressions. Scoping
the suppression attribute to a dedicated single-operation helper preserves
sanitizer coverage across the rest of the implementation.

## Examples

Bad:

```cpp
// Annotating entire function disables sanitizer checks across all statements
static VPX_NO_UNSIGNED_OVERFLOW_CHECK void highbd_variance64(
    const uint8_t *src8_ptr, int src_stride, const uint8_t *ref8_ptr,
    int ref_stride, int w, int h, uint64_t *sse, int64_t *sum) {
  ...
  const int diff = src[i] - ref[i];
  *sum += diff;
  *sse += (uint32_t)diff * (uint32_t)diff;
}
```

Good:

```cpp
// Isolate suppression attribute to minimal helper performing intentional overflow
static VPX_NO_UNSIGNED_OVERFLOW_CHECK uint32_t square_value(int value) {
  return (uint32_t)value * (uint32_t)value;
}

static void highbd_variance64(const uint8_t *src8_ptr, int src_stride,
                              const uint8_t *ref8_ptr, int ref_stride, int w,
                              int h, uint64_t *sse, int64_t *sum) {
  ...
  const int diff = src[i] - ref[i];
  *sum += diff;
  *sse += square_value(diff);
}
```
