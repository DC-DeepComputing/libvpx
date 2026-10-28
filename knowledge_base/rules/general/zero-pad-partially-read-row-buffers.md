---
name: libvpx/zero-pad-partially-read-row-buffers
description: When reading variable-length or truncated input into fixed-size row buffers, zero-pad trailing unread bytes using memset after copying available data
rubric: correctness
severity: should_fix
---

**Rule.** When reading or unpacking variable-length data into fixed-dimension
image rows or plane buffers, ensure partial reads copy available input bytes and
explicitly zero-pad the remainder of the row (e.g., memcpy(buf, data, nb)
followed by memset(buf + nb, 0, row_size - nb)). Do not only zero-fill when zero
bytes were read or leave trailing bytes uninitialized upon reaching EOF.

**Why.** If input or fuzz data ends mid-row, copying only the available bytes
without zero-padding the rest of the buffer leaves trailing memory
uninitialized, causing undefined behavior or non-deterministic execution in
downstream codecs.

## Examples

Bad:

```cpp
// Flawed: leaves trailing row memory uninitialized on partial row reads
if (nb > 0) {
  memcpy(buf, data, nb);
} else {
  memset(buf, 0, bytespp * w);
}
```

Good:

```cpp
// Correct: copies available bytes and zero-pads the rest of the row
memcpy(buf, data, nb);
memset(buf + nb, 0, bytespp * w - nb);
```
