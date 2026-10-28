---
name: libvpx/test/test-dynamic-resizing-transition-permutations
description: When testing dynamic frame resizing or spatial layer scaling, test multiple transition sequences and scale permutations rather than only one monotonic sequence
rubric: testing
severity: should_fix
---

**Rule.** When adding or updating tests for dynamic frame resizing, spatial
layer scaling, or resolution changes, parameterize tests to exercise multiple
transition orders and scaling permutations (e.g., 1/2 -> 1/4 -> full as well as
1/4 -> 1/2 -> full). Do not test only a single monotonic scaling sequence when
verifying dynamic resolution changes.

**Why.** Different transition sequences (e.g., progressively downscaling vs.
jumping directly to minimum resolution) exercise distinct layer enable/disable,
buffer reallocation, reference frame scaling, and rate control parameter update
code paths that a single monotonic sequence leaves untested.

## Examples

Bad:

```cpp
// Only tests one monotonic resize sequence (1/4 -> 1/2 -> full)
void ScaleForFrameNumber(unsigned int frame, unsigned int initial_w,
                         unsigned int initial_h, unsigned int *w,
                         unsigned int *h) {
  if (frame < 50) {
    *w = initial_w / 4;
    *h = initial_h / 4;
  } else if (frame < 100) {
    *w = initial_w / 2;
    *h = initial_h / 2;
  } else if (frame < 150) {
    *w = initial_w;
    *h = initial_h;
  }
}
```

Good:

```cpp
// Parameterized to test multiple resize sequences (e.g., 1/4 -> 1/2 -> 1 and 1/2 -> 1/4 -> 1)
void ScaleForFrameNumber(unsigned int frame, unsigned int initial_w,
                         unsigned int initial_h, unsigned int *w,
                         unsigned int *h, int resize_pattern) {
  if (resize_pattern == 1) {
    if (frame < 50) {
      *w = initial_w / 4;
      *h = initial_h / 4;
    } else if (frame < 100) {
      *w = initial_w / 2;
      *h = initial_h / 2;
    }  // ...
  } else if (resize_pattern == 2) {
    if (frame < 50) {
      *w = initial_w / 2;
      *h = initial_h / 2;
    } else if (frame < 100) {
      *w = initial_w / 4;
      *h = initial_h / 4;
    }  // ...
  }
}
```
