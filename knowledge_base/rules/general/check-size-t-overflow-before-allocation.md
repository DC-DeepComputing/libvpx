---
name: libvpx/check-size-t-overflow-before-allocation
description: When computing memory allocation sizes using multiplications or bit shifts on counts or dimensions, check against SIZE_MAX to prevent 32-bit integer overflow
rubric: correctness
severity: should_fix
---

**Rule.** When calculating memory allocation sizes or buffer capacities using
multiplications or bit shifts on counts, dimensions, or block sizes, validate
the input values against SIZE_MAX (e.g., count > SIZE_MAX / element_size) before
performing the calculation. Do not rely solely on casting factors to size_t to
prevent overflow, as size_t is 32 bits on 32-bit architectures. On overflow,
abort or report an error (such as via vpx_internal_error) rather than proceeding
with the allocation.

**Why.** On 32-bit platforms, size_t is 32 bits wide, so casting dimensions or
counts to size_t before multiplication or bit-shifting wraps around if the
product exceeds 2^32 - 1. Without an explicit division check against SIZE_MAX,
integer overflow results in undersized buffer allocations followed by heap
memory corruption or out-of-bounds writes.

## Examples

Bad:

```cpp
// Casting to size_t alone does not prevent overflow on 32-bit platforms.
const size_t dqcoeff_size = ((size_t)num_sbs << DQCOEFFS_PER_SB_LOG2) *
                            sizeof(*row_mt_worker_data->dqcoeff[0]);
row_mt_worker_data->dqcoeff[plane] = vpx_memalign(32, dqcoeff_size);
```

Good:

```cpp
// Explicitly check for size_t overflow before computing the allocation size.
if ((size_t)num_sbs > SIZE_MAX / (sizeof(*row_mt_worker_data->dqcoeff[0])
                                  << DQCOEFFS_PER_SB_LOG2)) {
  vpx_internal_error(&cm->error, VPX_CODEC_ERROR, "num_sbs too big");
}
const size_t dqcoeff_size = ((size_t)num_sbs << DQCOEFFS_PER_SB_LOG2) *
                            sizeof(*row_mt_worker_data->dqcoeff[0]);
row_mt_worker_data->dqcoeff[plane] = vpx_memalign(32, dqcoeff_size);
```
