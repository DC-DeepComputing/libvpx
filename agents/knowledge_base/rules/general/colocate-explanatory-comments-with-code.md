---
name: libvpx/colocate-explanatory-comments-with-code
description: Colocate explanatory and step comments directly adjacent to the specific variable, computation, or operation they document rather than detached at function start or above preparatory setup code
rubric: readability
severity: nit
---

**Rule.** Place and keep explanatory notes and step comments immediately
adjacent to the specific variable declaration, computation, control block, or
operation they document. Do not detach local implementation comments by placing
them at the beginning of an enclosing function or outer scope far from the
relevant logic. Additionally, when inserting preparatory setup code,
state-saving operations (such as `save_coding_context`), or conditional
pre-checks before an existing operation, keep comments attached directly before
the target operation (separated from preceding setup code by a blank line)
rather than placing them above the new setup statements.

**Why.** Placing comments immediately adjacent to the relevant variables and
operations provides immediate context to readers scanning the implementation and
prevents comments from becoming detached, obsolete, or ambiguous as surrounding
code evolves. Leaving step comments above newly inserted preparatory pre-checks
or setup code misleads readers into thinking the comment describes the setup
logic rather than the downstream operation it was written for.

## Examples

Bad:

```cpp
// Variant 1: Detached comment placed at function start rather than adjacent to logic
void vp9_get_smooth_motion_field(...) {
  // Note: search_mf and smooth_mf are scaled by macroblock size.
  FloatMV *input = (FloatMV *)malloc(...);
  ...
  for (row = 0; row < rows; ++row) {
    for (col = 0; col < cols; ++col) {
      const MV search_mv = search_mf[row * cols + col];
      FloatMV scaled_search_mv = { (float)search_mv.row / bh,
                                   (float)search_mv.col / bw };
    }
  }
}

// Variant 2: Comment placed above newly inserted pre-check instead of target operation
// build the bitstream
if (cpi->rc.use_post_encode_drop) save_coding_context(cpi);
vp9_pack_bitstream(cpi, dest, size);
```

Good:

```cpp
// Variant 1: Comment colocated directly with the relevant computation inside the loop
void vp9_get_smooth_motion_field(...) {
  FloatMV *input = (FloatMV *)malloc(...);
  ...
  for (row = 0; row < rows; ++row) {
    for (col = 0; col < cols; ++col) {
      // Note: search_mf and smooth_mf are scaled by macroblock size.
      const MV search_mv = search_mf[row * cols + col];
      FloatMV scaled_search_mv = { (float)search_mv.row / bh,
                                   (float)search_mv.col / bw };
    }
  }
}

// Variant 2: Pre-check separated and comment attached directly to the target call
if (cpi->rc.use_post_encode_drop) save_coding_context(cpi);

// build the bitstream
vp9_pack_bitstream(cpi, dest, size);
```
