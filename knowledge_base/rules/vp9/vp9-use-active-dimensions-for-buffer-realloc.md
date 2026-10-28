---
name: libvpx/vp9/vp9-use-active-dimensions-for-buffer-realloc
description: When checking whether to reallocate encoder buffers and updating dimension trackers, compare and assign active dimensions (cm->width, cm->height) rather than configuration values (oxcf)
rubric: correctness
severity: should_fix
---

**Rule.** When checking whether dynamic encoder or compressor buffers require
reallocation and updating tracked allocation dimensions (such as
cpi->initial_width and cpi->initial_height), evaluate and assign the active
common struct dimensions (cm->width, cm->height) that allocation routines
consume rather than configuration fields (cpi->oxcf.width, cpi->oxcf.height).

**Why.** Allocation routines (such as alloc_compressor_data) size buffers based
on active common dimensions (cm->width and cm->height), which may be scaled,
clamped, or aligned differently from configuration parameters (cpi->oxcf).
Evaluating or updating dimension trackers against oxcf causes desynchronization
between reallocation guards, tracked dimension bounds, and actual allocated
memory capacity.

## Examples

Bad:

```cpp
if (cpi->initial_width && cpi->initial_height &&
    (cpi->oxcf.width > cpi->initial_width ||
     cpi->oxcf.height > cpi->initial_height)) {
  vp9_free_context_buffers(cm);
  alloc_compressor_data(cpi);
  cpi->initial_width = cpi->oxcf.width;
  cpi->initial_height = cpi->oxcf.height;
}
```

Good:

```cpp
if (cpi->initial_width && cpi->initial_height &&
    (cm->width > cpi->initial_width || cm->height > cpi->initial_height)) {
  vp9_free_context_buffers(cm);
  alloc_compressor_data(cpi);
  cpi->initial_width = cm->width;
  cpi->initial_height = cm->height;
}
```
