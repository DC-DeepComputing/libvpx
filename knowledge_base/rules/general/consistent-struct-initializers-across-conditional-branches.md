---
name: libvpx/consistent-struct-initializers-across-conditional-branches
description: 'When defining struct initializers across #if and #else conditional compilation blocks, maintain consistent formatting, indentation, and member layout across all branches.'
rubric: readability
severity: nit
---

**Rule.** When initializing struct instances across `#if` and `#else` (or
`#elif`) conditional compilation branches, maintain consistent formatting,
indentation, and member layout across all branches. If one branch formats
members one per line or aligns them vertically, format the alternative branches
using the same style rather than mixing multiline and packed inline layouts.

**Why.** Inconsistent struct initializer layout across preprocessor branches
adds visual clutter and makes it difficult to compare differences between build
configurations and debug feature branches during code review and maintenance.

## Examples

Bad:

```cpp
#if CONFIG_MISMATCH_DEBUG
  struct encode_b_args arg = {
    x, enable_trellis_opt, trellis_opt_thresh, args->t_above, args->t_left,
    &mi->skip, 0, 0, 0
  };
#else
  struct encode_b_args arg = { x,
                                enable_trellis_opt,
                                trellis_opt_thresh,
                                args->t_above,
                                args->t_left,
                                &mi->skip };
#endif
```

Good:

```cpp
#if CONFIG_MISMATCH_DEBUG
  struct encode_b_args arg = { x,
                                enable_trellis_opt,
                                trellis_opt_thresh,
                                args->t_above,
                                args->t_left,
                                &mi->skip,
                                0,  // mi_row
                                0,  // mi_col
                                0 };  // output_enabled
#else
  struct encode_b_args arg = { x,
                                enable_trellis_opt,
                                trellis_opt_thresh,
                                args->t_above,
                                args->t_left,
                                &mi->skip };
#endif
```
