---
name: libvpx/vp8/use-check-mem-error-for-allocations
description: When allocating memory in VP8 or VP9 routines with an error context, wrap allocations in CHECK_MEM_ERROR and return void from allocation helpers
rubric: readability
severity: should_fix
---

**Rule.** When allocating memory via vpx_malloc(), vpx_calloc(), vpx_memalign(),
or vpx_realloc() in VP8 or VP9 routines that have access to a
vpx_internal_error_info error context (such as &cm->error or
&cpi->common.error), wrap the allocation in CHECK_MEM_ERROR(error, lval, expr).
Do not use manual null checks or ad-hoc error return codes for fallible
allocations. Additionally, define helper functions that perform allocations
using CHECK_MEM_ERROR with a void return type rather than returning dummy status
codes (such as int returning 0).

**Why.** In VP8 and VP9 codecs, CHECK_MEM_ERROR validates the allocation and
invokes vpx_internal_error() with VPX_CODEC_MEM_ERROR on failure, populating
error details and executing the codec's internal setjmp/longjmp error recovery
path. Because execution does not continue on allocation failure, manual null
checks, ad-hoc error codes, and dummy integer return values are redundant.

## Examples

Bad:

```cpp
// Manual null check and error return code
pbi->recon_map = (int8_t *)vpx_calloc(num_sbs, sizeof(*pbi->recon_map));
if (!pbi->recon_map) return 1;

// Returning dummy status code from helper using CHECK_MEM_ERROR
int vp9_dec_alloc_row_mt_mem(RowMTWorkerData *row_mt_worker_data,
                             VP9_COMMON *cm, int num_sbs) {
  CHECK_MEM_ERROR(cm, row_mt_worker_data->dqcoeff[0],
                  vpx_memalign(16, dqcoeff_size));
  return 0;
}
```

Good:

```cpp
// Wrap allocation in CHECK_MEM_ERROR
CHECK_MEM_ERROR(
    &cm->error, row_mt_worker_data->recon_map,
    vpx_calloc(num_sbs, sizeof(*row_mt_worker_data->recon_map)));

// Return void from helpers relying on CHECK_MEM_ERROR
void vp9_dec_alloc_row_mt_mem(RowMTWorkerData *row_mt_worker_data,
                              VP9_COMMON *cm, int num_sbs) {
  CHECK_MEM_ERROR(cm, row_mt_worker_data->dqcoeff[0],
                  vpx_memalign(16, dqcoeff_size));
}
```
