---
name: libvpx/name-extremum-variables-with-min-or-max-affix
description: When declaring variables storing minimum or maximum values (via VPXMIN/VPXMAX or std::min/max), use explicit min_/max_ prefixes or suffixes instead of bare names.
rubric: readability
severity: nit
---

**Rule.** Name intermediate, candidate, or local variables holding minimum or
maximum calculations (e.g., from VPXMIN, VPXMAX, fmin, fmax, std::min, or
std::max) with explicit min_ or max_ prefixes (e.g., min_start_row) or _min or
_max suffixes (e.g., start_row_min), rather than unqualified base property names
(e.g., start_row). Additionally, when an intermediate extremum is subject to
subsequent fallback resolution, clamping, or boundary validation, ensure the
identifier clearly reflects its candidate extremum status rather than the final
resolved property value.

**Why.** Using an unadorned property name for an extremum calculation obscures
the fact that the variable holds a boundary or candidate value rather than a
nominal or final resolved property. Explicitly tagging extremum variables with
min_ or max_ affixes prevents confusion and misleading code readability when
subsequent branching, ternary checks, clamping, or fallback logic conditionally
overrides the value.

## Examples

Bad:

```cpp
// Intermediate extremum uses unadorned property name before fallback resolution.
const int start_row = VPXMIN(this_tile->fp_data.image_data_start_row,
                             fp_acc_data->image_data_start_row);
this_tile->fp_data.image_data_start_row =
    (start_row == INVALID_ROW)
        ? VPXMAX(this_tile->fp_data.image_data_start_row,
                 fp_acc_data->image_data_start_row)
        : start_row;
```

Good:

```cpp
// Explicit prefix clearly identifies the candidate extremum value.
const int min_start_row = VPXMIN(this_tile->fp_data.image_data_start_row,
                                 fp_acc_data->image_data_start_row);
this_tile->fp_data.image_data_start_row =
    (min_start_row == INVALID_ROW)
        ? VPXMAX(this_tile->fp_data.image_data_start_row,
                 fp_acc_data->image_data_start_row)
        : min_start_row;
```
