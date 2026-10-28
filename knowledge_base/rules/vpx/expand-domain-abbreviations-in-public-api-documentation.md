---
name: libvpx/vpx/expand-domain-abbreviations-in-public-api-documentation
description: In documentation comments and Doxygen docstrings, expand abbreviations and identifier shorthand into descriptive prose, spell out words like 'number of', and uppercase domain acronyms (e.g., PSNR, CBR).
rubric: readability
severity: nit
---

**Rule.** In header documentation comments, Doxygen docstrings (such as \\param,
@param, \\brief, \\return, \\retval, or member /**< ... */ descriptions), and
source comments:
1. Write clear, descriptive plain English prose that spells out technical abbreviations, colloquial contractions, and truncated variable names (e.g., write 'number of' instead of 'num of', 'index' instead of 'idx', 'alternate reference' instead of 'arf', 'rate-distortion multiplier' instead of 'rdmult') rather than echoing code identifiers verbatim. Maintain consistent terminology across parameter descriptions within the same comment block.
2. Explicitly expand or define internal domain acronyms (such as 'Temporal dependency model' for TPL, or 'Group of Pictures' for GOP) upon first introduction in public API headers (vpx/*.h), structs, and enums.
3. Format standard domain acronyms and abbreviations (such as PSNR, SSIM, CBR, VBR, CQ, ARF, GF, or DCT) in full uppercase rather than lowercase or mixed-case representations (e.g., write 'PSNR' instead of 'psnr', 'CBR' instead of 'cbr').

**Why.** Documentation comments and Doxygen docstrings are intended for human
readers and API consumers. Merely echoing abbreviated identifier names (such as
'show_idx' or 'rdmult') or using informal colloquial shorthand (such as 'num
of') degrades clarity and professionalism. Expanding abbreviations into
descriptive prose and standardizing domain acronym capitalization (e.g.,
uppercase PSNR, CBR) avoids ambiguity, ensures consistency across public API
definitions, prevents confusion between acronyms and local variable names, and
produces clear, professional documentation.

## Examples

Bad:

```cpp
/*!\\brief Codec control function to set higher sharpness at the expense
 * of a lower psnr. Supported in cbr and vbr modes.
 */
VP8E_SET_SHARPNESS,

struct vpx_ext_ratectrl_config {
  int lag_in_frames; /**< num of lookahead source frames */
};

/*!\\brief Tpl stats for each block before propagation */
typedef struct VpxTplGopStats {
  VpxTplFrameStats *frame_stats_list; /**< List of tpl stats */
} VpxTplGopStats;

/*!\\brief Get frame rate control statistics.
 * \\param[in]  cpi              Pointer to cpi struct
 * \\param[in]  show_idx         Show_idx of the frame
 * \\param[in]  multi_layer_arf  Is multi-layer arf used
 * \\param[out] rdmult           Frame rdmult from model
 */
vpx_rc_status_t vpx_ext_rc_get_stats(vpx_rc_model_t cpi, int show_idx,
                                     int multi_layer_arf, int *rdmult);
```

Good:

```cpp
/*!\\brief Codec control function to set higher sharpness at the expense
 * of a lower PSNR. Supported in CBR and VBR modes.
 */
VP8E_SET_SHARPNESS,

struct vpx_ext_ratectrl_config {
  int lag_in_frames; /**< number of lookahead source frames */
};

/*!\\brief Temporal dependency model (TPL) stats for each block before propagation */
typedef struct VpxTplGopStats {
  VpxTplFrameStats *frame_stats_list; /**< List of temporal dependency stats */
} VpxTplGopStats;

/*!\\brief Get frame rate control statistics.
 * \\param[in]  cpi              Pointer to the controller instance
 * \\param[in]  show_idx         Show index of the frame
 * \\param[in]  multi_layer_arf  Is multi-layer alternate reference used
 * \\param[out] rdmult           Frame rate-distortion multiplier from model
 */
vpx_rc_status_t vpx_ext_rc_get_stats(vpx_rc_model_t cpi, int show_idx,
                                     int multi_layer_arf, int *rdmult);
```
