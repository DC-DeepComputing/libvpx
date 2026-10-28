---
name: libvpx/extract-repeated-plane-calculations-to-helper
description: When identical multi-line logic, formulas, or adjustment calculations are repeated across sibling functions, mode branches, or plane passes, factor them into a static helper function
rubric: design
severity: should_fix
---

**Rule.** Extract identical multi-line logic, state updates, or repeated
arithmetic adjustment formulas executed across multiple sibling functions,
conditional mode branches, or color planes into a shared static (or static
INLINE) helper function rather than copy-pasting the implementation inline.

**Why.** Duplicating non-trivial calculation formulas, state updates, or
adjustment logic across multiple functions, conditional mode branches, or color
planes increases maintenance burden, bloats code size, and risks subtle
divergence when future bug fixes or updates are not applied consistently to
every copy.

## Examples

Bad:

```cpp
// Variant 1: Duplicated plane calculations
modifier = VPXMIN(16, (modifier * 3 / index + rounding) >> strength);
modifier = (16 - modifier) * filter_weight;
// ... identical block repeated for u_mod and v_mod ...

// Variant 2: Duplicated logic across sibling mode handlers
if (cpi->svc.simulcast_mode &&
    cpi->svc.temporal_layer_id == cpi->svc.number_temporal_layers - 1 &&
    cpi->svc.temporal_layer_id > 0) {
  cpi->ext_refresh_last_frame = 0;
  cpi->ext_refresh_golden_frame = 0;
  cpi->ext_refresh_alt_ref_frame = 0;
}

// Variant 3: Duplicated formula across conditional mode branches
if (cpi->oxcf.rc_mode == VPX_CQ && gf_group->rf_level[i] == GF_ARF_LOW) {
  active_best_quality =
      ((layer_depth - 1) * cq_level + active_best_quality + layer_depth / 2) /
      layer_depth;
} else if (gf_group->rf_level[i] == GF_ARF_LOW) {
  active_best_quality =
      ((layer_depth - 1) * q + active_best_quality + layer_depth / 2) /
      layer_depth;
}
```

Good:

```cpp
// Variant 1: Shared static helper for plane calculations
static INLINE int mod_index(int sum_dist, int index, int rounding,
                            int strength, int filter_weight) {
  const int mod = VPXMIN(16, (sum_dist * 3 / index + rounding) >> strength);
  return (16 - mod) * filter_weight;
}
modifier = mod_index(modifier, index, rounding, strength, filter_weight);
u_mod = mod_index(u_mod, index, rounding, strength, filter_weight);

// Variant 2: Shared static helper for sibling mode handlers
static void non_reference_frame_simulcast(VP9_COMP *const cpi) {
  if (cpi->svc.temporal_layer_id == cpi->svc.number_temporal_layers - 1 &&
      cpi->svc.temporal_layer_id > 0) {
    cpi->ext_refresh_last_frame = 0;
    cpi->ext_refresh_golden_frame = 0;
    cpi->ext_refresh_alt_ref_frame = 0;
  }
}
if (cpi->svc.simulcast_mode) non_reference_frame_simulcast(cpi);

// Variant 3: Factored shared formula into helper and unified branches
static int adjust_layer_depth_quality(int base_q, int active_best_quality,
                                      int layer_depth) {
  return ((layer_depth - 1) * base_q + active_best_quality + layer_depth / 2) /
         layer_depth;
}
if (gf_group->rf_level[i] == GF_ARF_LOW) {
  const int base_q = (cpi->oxcf.rc_mode == VPX_CQ) ? cq_level : q;
  active_best_quality =
      adjust_layer_depth_quality(base_q, active_best_quality,
                                 gf_group->layer_depth[i]);
}
```
