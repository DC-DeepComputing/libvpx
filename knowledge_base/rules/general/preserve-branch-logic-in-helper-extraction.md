---
name: libvpx/preserve-branch-logic-in-helper-extraction
description: When extracting duplicated logic into a shared helper function, verify all mode-specific branches, AQ/content overrides, and edge cases from original call sites are preserved
rubric: correctness
severity: should_fix
---

**Rule.** When refactoring duplicated code blocks or extracting repeated logic
across multiple call sites into a shared helper function, compare the extracted
function against every original call site to ensure all conditional checks,
special-case overrides, mode-specific branches (such as screen content
`VP9E_CONTENT_SCREEN` or adaptive quantization `CYCLIC_REFRESH_AQ` /
`TX_MODE_SELECT`), and caller-specific parameters are fully preserved. If a
condition or parameter only applies to certain callers, pass the necessary
context parameters or retain caller-specific adjustments.

**Why.** Refactoring duplicated code into a common helper can easily drop subtle
edge-case branches or content overrides present at only some call sites,
silently altering encoder behavior and causing rate-distortion performance or
functional regressions.

## Examples

Bad:

```cpp
// Extracted helper dropped the screen-content branch (VP9E_CONTENT_SCREEN) present at original call sites
static TX_SIZE calculate_tx_size(VP9_COMP *cpi, BLOCK_SIZE bsize,
                                 MACROBLOCKD *xd, unsigned int var,
                                 unsigned int sse) {
  TX_SIZE tx_size;
  if (cpi->common.tx_mode == TX_MODE_SELECT) {
    if (sse > (var << 2))
      tx_size = VPXMIN(max_txsize_lookup[bsize],
                       tx_mode_to_biggest_tx_size[cpi->common.tx_mode]);
    else
      tx_size = TX_8X8;
    if (cpi->oxcf.aq_mode == CYCLIC_REFRESH_AQ &&
        cyclic_refresh_segment_id_boosted(xd->mi[0]->segment_id))
      tx_size = TX_8X8;
    else if (tx_size > TX_16X16)
      tx_size = TX_16X16;
  } else {
    tx_size = VPXMIN(max_txsize_lookup[bsize],
                     tx_mode_to_biggest_tx_size[cpi->common.tx_mode]);
  }
  return tx_size;
}
```

Good:

```cpp
// Preserves all mode checks and caller context, including the screen-content condition
static TX_SIZE calculate_tx_size(VP9_COMP *const cpi, BLOCK_SIZE bsize,
                                 MACROBLOCKD *const xd, unsigned int var,
                                 unsigned int sse, int64_t ac_thr) {
  TX_SIZE tx_size;
  if (cpi->common.tx_mode == TX_MODE_SELECT) {
    if (sse > (var << 2))
      tx_size = VPXMIN(max_txsize_lookup[bsize],
                       tx_mode_to_biggest_tx_size[cpi->common.tx_mode]);
    else
      tx_size = TX_8X8;
    if (cpi->oxcf.aq_mode == CYCLIC_REFRESH_AQ &&
        cyclic_refresh_segment_id_boosted(xd->mi[0]->segment_id))
      tx_size = TX_8X8;
    else if (tx_size > TX_16X16)
      tx_size = TX_16X16;
    if (cpi->oxcf.content == VP9E_CONTENT_SCREEN && tx_size == TX_8X8 &&
        bsize <= BLOCK_16X16 && ((var >> 5) > (unsigned int)ac_thr))
      tx_size = TX_4X4;
  } else {
    tx_size = VPXMIN(max_txsize_lookup[bsize],
                     tx_mode_to_biggest_tx_size[cpi->common.tx_mode]);
  }
  return tx_size;
}
```
