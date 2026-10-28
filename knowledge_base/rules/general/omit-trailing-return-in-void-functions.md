---
name: libvpx/omit-trailing-return-in-void-functions
description: When completing execution in void functions, omit redundant trailing return statements before the closing brace and reserve explicit return for early exits
rubric: readability
severity: nit
---

**Rule.** Do not include an explicit `return;` statement as the final statement
of a `void` function body. Allow execution to return naturally at the closing
brace, reserving explicit `return;` statements solely for early exits and guard
clauses.

**Why.** In C and C++, execution returns implicitly upon reaching the closing
brace of a `void` function. A trailing `return;` statement is superfluous
boilerplate that adds visual clutter without altering control flow or runtime
semantics.

## Examples

Bad:

```cpp
// C / SIMD function with trailing return
void vp8_sixtap_predict4x4_neon(const uint8_t *src, int src_pitch,
                                uint8_t *dst, int dst_pitch) {
  store4x4(dst, dst_pitch, d0, d1);
  return;
}

// Function with guard clause and redundant final return
void ScaleForFrameNumber(unsigned int frame, unsigned int initial_w,
                         unsigned int initial_h, unsigned int *w,
                         unsigned int *h) {
  if (frame < 50) {
    *w = initial_w / 4;
    *h = initial_h / 4;
  }
  return;
}
```

Good:

```cpp
// C / SIMD function omitting trailing return
void vp8_sixtap_predict4x4_neon(const uint8_t *src, int src_pitch,
                                uint8_t *dst, int dst_pitch) {
  store4x4(dst, dst_pitch, d0, d1);
}

// Function with guard clause and natural exit
void ScaleForFrameNumber(unsigned int frame, unsigned int initial_w,
                         unsigned int initial_h, unsigned int *w,
                         unsigned int *h) {
  if (frame < 50) {
    *w = initial_w / 4;
    *h = initial_h / 4;
  }
}
```
