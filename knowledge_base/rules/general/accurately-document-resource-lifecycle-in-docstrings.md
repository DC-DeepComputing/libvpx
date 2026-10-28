---
name: libvpx/accurately-document-resource-lifecycle-in-docstrings
description: When documenting functions that accept opened handles or preallocated buffers, accurately describe resource lifecycle and inputs rather than stating the function opens or allocates them
rubric: readability
severity: should_fix
---

**Rule.** Do not state in function docstrings or documentation comments that a
function opens, allocates, or creates a resource when it accepts an
already-opened handle, preallocated buffer, or existing pointer (such as an
opened FILE *). Accurately document expected input state and caller resource
ownership responsibilities.

**Why.** Inaccurate doc comments mislead API consumers and internal callers
about resource ownership and preconditions, causing confusion over who is
responsible for acquiring or freeing resources and introducing contradictions
between high-level function summaries and parameter descriptions.

## Examples

Bad:

```cpp
/*!\brief Write VpxTplGopStats to file
 *
 * Opens a file and writes VpxTplGopStats.
 *
 * \param[in]  tpl_file  A FILE pointer that has already been opened.
 */
vpx_codec_err_t vpx_write_tpl_stats(FILE *tpl_file,
                                    const VpxTplGopStats *tpl_gop_stats);
```

Good:

```cpp
/*!\brief Write VpxTplGopStats to file
 *
 * Accepts an opened file handle and writes VpxTplGopStats.
 *
 * \param[in]  tpl_file  A FILE pointer that has already been opened.
 */
vpx_codec_err_t vpx_write_tpl_stats(FILE *tpl_file,
                                    const VpxTplGopStats *tpl_gop_stats);
```
