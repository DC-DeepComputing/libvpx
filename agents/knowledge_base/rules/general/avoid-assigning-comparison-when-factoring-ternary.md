---
name: libvpx/avoid-assigning-comparison-when-factoring-ternary
rubric: correctness
severity: blocking
---

**Rule.** When extracting a repeated subexpression from a conditional or ternary
expression into a local variable, initialize the variable with the evaluated
operand value rather than the boolean comparison. Keep the comparison against
sentinel or boundary values in the conditional check, not in the variable
initializer.

**Why.** Accidentally including the comparison operator in the extracted
variable initialization causes the variable to store a boolean (0 or 1) rather
than the evaluated operand value. Subsequent sentinel comparisons will evaluate
incorrectly, and ternary fallback branches will return boolean flags instead of
the computed value.

## Examples

Bad:

```cpp
// Accidental inclusion of comparison operator in extracted variable initialization:
const int start_row =
    VPXMIN(tile_data->fp_data.image_data_start_row,
           tile_data_t->fp_data.image_data_start_row) == INVALID_ROW;
tile_data->fp_data.image_data_start_row =
    (start_row == INVALID_ROW)
        ? VPXMAX(tile_data->fp_data.image_data_start_row,
                 tile_data_t->fp_data.image_data_start_row)
        : start_row;
```

Good:

```cpp
// Extract the evaluated operand into the variable, keeping comparison in the ternary:
const int min_start_row =
    VPXMIN(tile_data->fp_data.image_data_start_row,
           tile_data_t->fp_data.image_data_start_row);
tile_data->fp_data.image_data_start_row =
    (min_start_row == INVALID_ROW)
        ? VPXMAX(tile_data->fp_data.image_data_start_row,
                 tile_data_t->fp_data.image_data_start_row)
        : min_start_row;
```
