---
name: libvpx/doxygen-prefix-macro-references-with-hash
description: 'When referencing symbols, struct members, or function arguments in Doxygen comments, prefix symbols with # for autolinks, use \p for parameter names, and avoid directional terms'
rubric: readability
severity: nit
---

**Rule.** In Doxygen documentation comments (such as /*!\brief ... */, /**< ...
*/, and \retval) within headers, prefix referenced macro, enum, constant,
function, and struct member names with # (e.g., #VPX_SS_MAX_LAYERS,
#VPX_CODEC_OK, #index) for autolinking, and refer to function arguments by
parameter name formatted with \p (e.g., \p tpl_gop_stats) rather than by type
name. Avoid directional terms like 'above' or 'below' when referencing adjacent
struct fields or declarations; use explicit, #-prefixed symbol names instead.

**Why.** Doxygen uses the # prefix on symbols and member names to generate
automatic hyperlinks, and \p to render parameter identifiers in teletype code
font. Omitting # leaves symbol names as unlinked plain text, directional terms
like 'above' or 'below' become stale when declarations are reordered, and
referencing arguments by type name rather than parameter identifier obscures
which specific argument is being operated on.

## Examples

Bad:

```cpp
// Bad: un-prefixed macro/enum references, directional terms, and type name instead of \p param
/*!\brief Codec control to get last quantizers for all spatial layers.
 * The size of the array passed in should be VPX_SS_MAX_LAYERS.
 * \retval VPX_CODEC_OK on success.
 */
VP9E_GET_LAST_QUANTIZER_SVC_LAYERS,

/*!
 * Ref frame name. This decides whether the index above is used as
 * |lst_fb_idx|, |gld_fb_idx| or |alt_fb_idx| in VP9_COMP.
 */
vpx_rc_ref_name_t name[VPX_RC_MAX_REF_FRAMES];

/*!\brief Read VpxTplGopStats from file
 *
 * Accepts an opened file handle and reads TPL stats into VpxTplGopStats.
 */
vpx_codec_err_t vpx_read_tpl_stats(FILE *tpl_file,
                                   VpxTplGopStats *tpl_gop_stats);
```

Good:

```cpp
// Good: prefixed with # for Doxygen autolinks, explicit member names, and \p parameter names
/*!\brief Codec control to get last quantizers for all spatial layers.
 * The size of the array passed in should be #VPX_SS_MAX_LAYERS.
 * \retval #VPX_CODEC_OK on success.
 */
VP9E_GET_LAST_QUANTIZER_SVC_LAYERS,

/*!
 * Ref frame name. This decides whether the #index is used as
 * |lst_fb_idx|, |gld_fb_idx| or |alt_fb_idx| in VP9_COMP.
 */
vpx_rc_ref_name_t name[VPX_RC_MAX_REF_FRAMES];

/*!\brief Read VpxTplGopStats from file
 *
 * Accepts an opened file handle and reads TPL stats into \p tpl_gop_stats.
 */
vpx_codec_err_t vpx_read_tpl_stats(FILE *tpl_file,
                                   VpxTplGopStats *tpl_gop_stats);
```
