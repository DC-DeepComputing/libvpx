---
name: libvpx/zero-entire-array-buffer-not-single-element
description: When zero-initializing an array buffer or interior pointer with memset, ensure the size covers the entire buffer rather than a single dereferenced element
rubric: correctness
severity: should_fix
---

**Rule.** When zero-initializing an array buffer using memset (or subsystem zero
macros like vp9_zero_array or vp8_zero_array), ensure the size passed covers the
entire buffer extent (e.g., sizeof(buf) or count * sizeof(*ptr)) rather than
just a single dereferenced element (sizeof(*ptr)). Pay special attention when
referencing an offset or interior pointer within the buffer: zero the underlying
base buffer or compute the remaining buffer extent from the offset, rather than
only clearing the first indexed element.

**Why.** Passing sizeof(*ptr) to memset clears only a single element (sizeof(T))
rather than the full array allocation. When subsequent operations or
negative/positive indexing access other elements in the buffer, reading
uninitialized slots yields indeterminate garbage values and causes subtle
encoding bugs or undefined behavior.

## Examples

Bad:

```cpp
GF_PICTURE gf_picture_buf[MAX_ARF_GOP_SIZE + REFS_PER_FRAME];
GF_PICTURE *gf_picture = &gf_picture_buf[REFS_PER_FRAME];

// Bug: Only zeroes a single GF_PICTURE element instead of the buffer.
memset(gf_picture, 0, sizeof(*gf_picture));
```

Good:

```cpp
GF_PICTURE gf_picture_buf[MAX_ARF_GOP_SIZE + REFS_PER_FRAME];
GF_PICTURE *gf_picture = &gf_picture_buf[REFS_PER_FRAME];

// Correct: Zeroes the entire allocated array buffer.
memset(gf_picture_buf, 0, sizeof(gf_picture_buf));
```
