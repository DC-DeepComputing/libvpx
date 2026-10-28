---
name: libvpx/prefer-calloc-over-malloc-and-memset
description: When allocating zero-initialized memory buffers, prefer calloc() or vpx_calloc() over pairing malloc() or vpx_malloc() with a subsequent memset() zeroing call
rubric: readability
severity: nit
---

**Rule.** When allocating dynamic memory or buffer storage that requires
zero-initialization, use calloc() or vpx_calloc() directly instead of calling
malloc() or vpx_malloc() followed by a separate memset(..., 0, ...) call. This
applies to direct pointer assignments as well as CHECK_MEM_ERROR() macro
invocations. Ensure allocation results are verified before dereferencing.

**Why.** Using calloc() or vpx_calloc() combines allocation and zeroing into a
single operation, eliminating redundant boilerplate code, avoiding potential
count/size calculation mismatches between the allocation and the subsequent
memset(), and eliminating the risk of calling memset() on a null pointer if the
allocation fails.

## Examples

Bad:

```cpp
// Anti-pattern 1: malloc followed by separate memset zeroing.
uint8_t *intbuf = (uint8_t *)malloc(sizeof(*intbuf) * width2 * height);
memset(intbuf, 0, sizeof(*intbuf) * width2 * height);

// Anti-pattern 2: vpx_malloc paired with memset in CHECK_MEM_ERROR.
CHECK_MEM_ERROR(&cm->error, cm->mip,
                vpx_malloc(mi_size * sizeof(*cm->mip)));
memset(cm->mip, 0, mi_size * sizeof(*cm->mip));
```

Good:

```cpp
// Idiomatic: calloc directly zero-initializes allocated memory.
uint8_t *intbuf = (uint8_t *)calloc(width2 * height, sizeof(*intbuf));

// Idiomatic: vpx_calloc directly combined with CHECK_MEM_ERROR.
CHECK_MEM_ERROR(&cm->error, cm->mip,
                vpx_calloc(mi_size, sizeof(*cm->mip)));
```
