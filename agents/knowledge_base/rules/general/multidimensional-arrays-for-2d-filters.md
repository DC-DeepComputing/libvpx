---
name: libvpx/multidimensional-arrays-for-2d-filters
description: Declare fixed 2D arrays, filters, and buffer parameters using multidimensional array or pointer-to-array syntax instead of flattened 1D pointers with manual stride arithmetic
rubric: readability
severity: should_fix
---

**Rule.** Declare fixed-size 2D spatial filters, convolution kernels, or lookup
matrices as multidimensional arrays (e.g., `type filter[ROWS][COLS]`) and index
them directly as `filter[r][c]`. Similarly, when declaring function parameters,
struct members, or local aliases for multidimensional buffers or tables with a
fixed inner dimension (such as N×K elements), use pointer-to-array syntax (e.g.,
`const T (*param)[K]`) and index with `param[idx][k]`. Do not flatten fixed 2D
matrices or buffers into 1D arrays or flat pointers (`const T *param`) requiring
manual stride calculations like `filter[r * COLS + c]` or `param[idx * K + k]`.

**Why.** Declaring multidimensional array dimensions and pointer-to-array types
makes geometric layout and coordinate bounds explicit, enables compiler-enforced
dimensional type safety, eliminates error-prone manual stride arithmetic, and
clearly communicates expected buffer dimensions at API boundaries.

## Examples

Bad:

```cpp
// Bad: Flattened 2D filter array with manual stride arithmetic
float filter[9] = { 1.0f / 12.0f, 1.0f / 6.0f, 1.0f / 12.0f,
                    1.0f / 6.0f,  0.0f,        1.0f / 6.0f,
                    1.0f / 12.0f, 1.0f / 6.0f, 1.0f / 12.0f };
avg_nb_mv[0] += (tmp_mv.row) * filter[idx_row * 3 + idx_col];

// Bad: Flat pointer requiring manual stride arithmetic for fixed inner dimension
MV get_smooth_mv(const MV *search_mv, const int *M, int rows, int cols) {
  float M00 = M[ref_row * cols * 4 + ref_col * 4];
  float M01 = M[ref_row * cols * 4 + ref_col * 4 + 1];
}
```

Good:

```cpp
// Good: Multidimensional array with [r][c] indexing
float filter[3][3] = { { 1.0f / 12.0f, 1.0f / 6.0f, 1.0f / 12.0f },
                       { 1.0f / 6.0f,  0.0f,        1.0f / 6.0f },
                       { 1.0f / 12.0f, 1.0f / 6.0f, 1.0f / 12.0f } };
avg_nb_mv[0] += (tmp_mv.row) * filter[idx_row][idx_col];

// Good: Pointer-to-array type encodes inner dimension with clean 2D indexing
static MV get_smooth_mv(const MV search_mv, const int (*M)[4], int rows, int cols) {
  float M00 = M[ref_row * cols + ref_col][0];
  float M01 = M[ref_row * cols + ref_col][1];
}
```
