---
name: libvpx/check-64bit-allocation-sizes-against-size-max
description: When passing 64-bit integer counts or allocation sizes to size_t allocator functions, validate bounds against SIZE_MAX and explicitly cast to (size_t)
rubric: correctness
severity: should_fix
---

**Rule.** When computing buffer element counts, dimensions, or allocation sizes
using 64-bit integer types (int64_t or uint64_t) and passing them to allocator
functions expecting size_t arguments (such as vpx_calloc(), vpx_malloc(),
vpx_realloc(), or vpx_memalign()):
1. Verify that the value fits safely within size_t (e.g., checking tokens < 0 || (uint64_t)tokens > SIZE_MAX). Guard the check with #if INT64_MAX > SIZE_MAX or #if UINT64_MAX > SIZE_MAX where appropriate to avoid tautological comparison warnings on 64-bit builds.
2. Handle out-of-range values by returning an error status or calling vpx_internal_error() with VPX_CODEC_MEM_ERROR.
3. Explicitly cast the value to (size_t) when passing it to the allocation function to prevent compiler conversion warnings (such as MSVC C4244).

**Why.** On 32-bit platforms, size_t is 32 bits whereas int64_t and uint64_t are
64 bits. Passing an unchecked 64-bit integer to an allocator function expecting
size_t silently truncates higher bits, allocating an undersized buffer and
causing heap buffer overflows or memory corruption instead of reporting an
out-of-memory error. Furthermore, passing 64-bit integers directly triggers MSVC
compiler warning C4244 ('conversion from int64_t to size_t, possible loss of
data'); explicitly casting with (size_t) after bounds validation avoids compiler
warnings across 32-bit build targets.

## Examples

Bad:

```cpp
// Bad: Passing unchecked 64-bit count to size_t allocator causes truncation
// on 32-bit platforms and triggers MSVC compiler warning C4244.
int64_t tokens = get_token_alloc(cm->mb_rows, cm->mb_cols);
CHECK_MEM_ERROR(&cm->error, cpi->tile_tok[0][0],
                vpx_calloc(tokens, sizeof(*cpi->tile_tok[0][0])));
```

Good:

```cpp
// Good: Guard check against SIZE_MAX on 32-bit builds, report memory error,
// and explicitly cast to (size_t).
int64_t tokens = get_token_alloc(cm->mb_rows, cm->mb_cols);
#if INT64_MAX > SIZE_MAX
if (tokens < 0 || (uint64_t)tokens > SIZE_MAX) {
  vpx_internal_error(
      &cm->error, VPX_CODEC_MEM_ERROR,
      "Size of cpi->tile_tok[0][0] can't be represented in size_t");
}
#endif
CHECK_MEM_ERROR(&cm->error, cpi->tile_tok[0][0],
                vpx_calloc((size_t)tokens, sizeof(*cpi->tile_tok[0][0])));
```
