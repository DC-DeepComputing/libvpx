---
name: libvpx/align-multiline-doxygen-descriptions
description: When wrapping multi-line Doxygen parameter or return descriptions in header doc comments, align continuation lines with the start column of the description text
rubric: readability
severity: nit
---

**Rule.** In Doxygen documentation comments (such as \param, \return, or
\retval), indent and align wrapped multi-line continuation text to match the
starting column of the parameter or return description. Do not leave wrapped
continuation lines unaligned at the beginning of the comment line.

**Why.** Aligning wrapped continuation lines with the start of the description
column maintains clean visual indentation and readability in header files,
making parameter names, direction tags, and descriptions easy to distinguish.

## Examples

Bad:

```cpp
/*!\brief Process temporal dependency model stats.
 *
 * \param[in]    tpl_gop_stats  VpxTplGopStats that contains TPL stats for the
 * whole GOP.
 * \return Status code indicating success or error.
 */
vpx_codec_err_t vpx_process_tpl_stats(const VpxTplGopStats *tpl_gop_stats);
```

Good:

```cpp
/*!\brief Process temporal dependency model stats.
 *
 * \param[in]    tpl_gop_stats  VpxTplGopStats that contains TPL stats for the
 *                              whole GOP.
 * \return Status code indicating success or error.
 */
vpx_codec_err_t vpx_process_tpl_stats(const VpxTplGopStats *tpl_gop_stats);
```
