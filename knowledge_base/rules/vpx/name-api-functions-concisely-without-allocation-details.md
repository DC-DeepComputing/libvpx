---
name: libvpx/vpx/name-api-functions-concisely-without-allocation-details
description: When declaring public API I/O, deserialization, or parsing functions, name them concisely (e.g., vpx_read_) instead of encoding internal allocation mechanics like alloc_and_read
rubric: readability
severity: nit
---

**Rule.** When naming public API functions for deserialization, parsing, or I/O,
use concise operation names (such as vpx_read_<type> paired with
vpx_write_<type>) rather than embedding internal memory allocation details like
alloc_and_read in the identifier. Document dynamic allocations, member buffer
lifecycles, and cleanup/deallocation obligations in the function's Doxygen doc
comments instead.

**Why.** Encoding internal allocation mechanics into public API function names
creates verbose and asymmetric identifiers and exposes implementation details
that may change. Describing memory ownership and allocation contracts in doc
comments keeps public API identifiers clean, consistent, and symmetric across
read and write operations.

## Examples

Bad:

```cpp
/* Awkward function name embeds internal allocation behavior */
vpx_codec_err_t vpx_alloc_and_read_tpl_stats(FILE *tpl_file,
                                             VpxTplGopStats *tpl_gop_stats);
```

Good:

```cpp
/*!\brief Read VpxTplGopStats from file
 *
 * Opens a file and reads TPL stats into VpxTplGopStats. Note that
 * internal buffers (such as frame_stats_list) are dynamically allocated
 * and must be freed by the caller using vpx_free_tpl_gop_stats().
 */
vpx_codec_err_t vpx_read_tpl_stats(FILE *tpl_file,
                                   VpxTplGopStats *tpl_gop_stats);
```
