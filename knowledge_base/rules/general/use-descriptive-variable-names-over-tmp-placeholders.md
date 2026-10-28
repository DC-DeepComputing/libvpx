---
name: libvpx/use-descriptive-variable-names-over-tmp-placeholders
description: When declaring variables for computed metrics, values, or intermediate results, use descriptive names reflecting the entity and metric instead of generic tmp_* placeholders
rubric: readability
severity: nit
---

**Rule.** When declaring variables for computed values, metrics, or intermediate
calculations, use descriptive names that reflect the specific entity and metric
being calculated (e.g., `block_sad`, `frame_variance`, `pixel_diff`). Avoid
generic temporary prefixes or placeholder names like `tmp_*`, `temp_*`, or
`var_*`.

**Why.** Generic placeholder names like `tmp_sad` or `temp_val` obscure the
meaning and scope of intermediate calculations. Descriptive identifiers clarify
which entity, block, or frame the metric belongs to and prevent accidental
variable reuse.

## Examples

Bad:

```cpp
uint64_t tmp_sad;
tmp_sad = cpi->fn_ptr[bsize].sdf(src_y, ystride, last_src_y, last_ystride);
if (tmp_sad == 0) return 1;
```

Good:

```cpp
uint64_t block_sad;
block_sad = cpi->fn_ptr[bsize].sdf(src_y, ystride, last_src_y, last_ystride);
if (block_sad == 0) return 1;
```
