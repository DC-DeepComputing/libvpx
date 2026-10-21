---
name: libvpx/store-computed-index-and-assert-bounds
rubric: safety
severity: should_fix
---

**Rule.** When calculating array or buffer indices using arithmetic adjustments,
subtractions, base offsets, or deltas, store the final resolved index in a
dedicated local variable (e.g., `const int idx = base + delta;`) and explicitly
assert or validate its bounds (e.g., `assert(index >= 0)` or `assert(idx >= 0 &&
idx < size)`) before subscripting into the array. Do not assert on intermediate
delta ranges alone or embed composite arithmetic expressions directly inside
array subscript brackets.

**Why.** Array indices adjusted via arithmetic offsets or subtractions rely on
non-obvious upstream preconditions that can break as surrounding code evolves.
Embedding arithmetic directly inside subscript brackets obscures bounds checking
and duplicates computation. Storing the computed index in a dedicated variable
and asserting on its final range directly verifies the exact value used for
memory access, preventing out-of-bounds indexing bugs and documenting range
invariants.

## Examples

Bad:

```cpp
// Variant 1: Arithmetic adjustment without non-negative bounds assertion
int index = svc->spatial_layer_id;
if (svc->number_spatial_layers == 3) {
  index = svc->spatial_layer_id - 1;
}
cpi->alt_fb_idx = svc->buffer_ref[index].idx;

// Variant 2: Asserting intermediate delta instead of final index
const int row_val = ((mv.row - ref_mv.row) >> 1);
assert(row_val >= -mv_max && row_val <= mv_max);
x->MVcount[0][mv_max + row_val]++;
```

Good:

```cpp
// Variant 1: Explicit assertion after arithmetic adjustment
int index = svc->spatial_layer_id;
if (svc->number_spatial_layers == 3) {
  index = svc->spatial_layer_id - 1;
}
assert(index >= 0);
cpi->alt_fb_idx = svc->buffer_ref[index].idx;

// Variant 2: Store final index and assert computed bounds
const int row_val = ((mv.row - ref_mv.row) >> 1);
const int row_idx = mv_max + row_val;
assert(row_idx >= 0 && row_idx < MVvals);
x->MVcount[0][row_idx]++;
```
