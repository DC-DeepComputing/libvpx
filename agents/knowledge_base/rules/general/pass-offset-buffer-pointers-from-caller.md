---
name: libvpx/pass-offset-buffer-pointers-from-caller
description: When helper functions operate on buffer sub-slices or stages, index relative to zero inside the helper and pass offset pointers at call sites
rubric: design
severity: should_fix
---

**Rule.** When a helper function operates on a sub-block, stage, or slice of an
array or buffer (e.g., in transform or filter pipelines), design the helper to
index relative to zero (`in[0..N-1]`, `out[0..N-1]`) rather than embedding
non-zero offsets. Pass offset buffer pointers (e.g., `&buf[offset]` or `buf +
offset`) from caller call sites.

**Why.** Hardcoding non-zero buffer offsets inside helper functions implicitly
couples the callee to the caller's specific buffer layout and obscures buffer
boundaries and stage sizes. Zero-based indexing inside helpers makes functions
modular, reusable, and easier to verify against buffer overruns while
documenting explicit parameter bounds.

## Examples

Bad:

```cpp
// Helper/callee hardcodes non-zero offsets into caller's full buffer:
static void quarter_2(const __m128i *in, __m128i *out) {
  out[8] = _mm_add_epi16(in[0], in[1]);
  out[9] = _mm_sub_epi16(in[2], in[3]);
}

// Caller passes base buffer pointer:
quarter_2(in, temp);
```

Good:

```cpp
// Helper/callee indexes sub-slice relative to zero:
static void quarter_2(const __m128i *in, __m128i *out) {
  out[0] = _mm_add_epi16(in[0], in[1]);
  out[1] = _mm_sub_epi16(in[2], in[3]);
}

// Caller explicitly passes pointer to sub-slice offset:
quarter_2(in, &temp[8]);
```
