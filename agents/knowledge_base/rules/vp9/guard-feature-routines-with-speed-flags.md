---
name: libvpx/vp9/guard-feature-routines-with-speed-flags
description: When invoking feature-specific analysis, setup, or processing routines in encoder passes, guard callsites with the governing speed-feature or configuration flags
rubric: correctness
severity: should_fix
---

**Rule.** Guard feature-specific data preparation, analysis passes, and
processing routines with the corresponding speed-feature, configuration, or
usage flag (such as cpi->sf.<feature>, cpi->oxcf.<mode>, or cpi->use_<feature>).
Do not invoke feature-specific helpers unconditionally when the governing
feature is disabled.

**Why.** Invoking feature-specific computation unconditionally causes wasted CPU
cycles when the feature is turned off and risks operating on unallocated, empty,
or uninitialized data structures.

## Examples

Bad:

```cpp
// Bad: executes feature-specific clustering unconditionally even when
// the feature is disabled.
vp9_kmeans(cpi->kmeans_ctr_ls, cpi->kmeans_ctr_num, cpi->kmeans_data_arr,
           cpi->kmeans_data_size);
```

Good:

```cpp
// Good: gated on the governing speed feature and frame display condition.
if (cpi->sf.enable_wiener_variance && cm->show_frame) {
  vp9_kmeans(cpi->kmeans_ctr_ls, cpi->kmeans_ctr_num, cpi->kmeans_data_arr,
             cpi->kmeans_data_size);
}
```
