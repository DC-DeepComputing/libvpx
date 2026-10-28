---
name: libvpx/validate-array-bounds-before-writing-and-incrementing
description: When accumulating or writing elements into fixed-capacity arrays or buffers, validate count and index bounds against capacity before accessing, writing, or incrementing.
rubric: safety
severity: blocking
---

**Rule.** Verify that array indices and container counts are within valid bounds
and below maximum capacity limits (e.g., `count >= MAX_CAPACITY` or `count >=
MAX_PARTITIONS`) before performing array access, element writes, or counter
increments. Do not write to an array slot or increment element counts prior to
validating bounds with the expectation of checking bounds or rolling back
afterward.

**Why.** Accessing or modifying array elements before validating index bounds
allows out-of-bounds reads or writes (buffer overflows) when the buffer is
already full. Furthermore, checking bounds post-increment creates an off-by-one
hazard: testing `count >= MAX_CAPACITY` after incrementing rejects a valid final
element, whereas testing `count > MAX_CAPACITY` causes an out-of-bounds write
before failing. Validating capacity bounds prior to indexing guarantees that
only valid buffer slots are accessed and prevents memory corruption.

## Examples

Bad:

```cpp
// Writing and incrementing count before checking bounds or attempting rollback
ctx->fragments.ptrs[ctx->fragments.count] = data;
ctx->fragments.sizes[ctx->fragments.count] = data_sz;
ctx->fragments.count++;
if (ctx->fragments.count >= MAX_PARTITIONS) {
  ctx->fragments.count = 0;
  *res = VPX_CODEC_INVALID_PARAM;
  return -1;
}
```

Good:

```cpp
// Validate capacity bounds before writing elements or incrementing count
if (ctx->fragments.count >= MAX_PARTITIONS) {
  ctx->fragments.count = 0;
  *res = VPX_CODEC_INVALID_PARAM;
  return -1;
}
ctx->fragments.ptrs[ctx->fragments.count] = data;
ctx->fragments.sizes[ctx->fragments.count] = data_sz;
ctx->fragments.count++;
```
