---
name: libvpx/prefer-scalar-temporaries-over-intermediate-array-writes
description: When computing intermediate values or multi-step transformations on array or buffer elements, use local scalar variables instead of writing intermediate or dead results to array memory
rubric: readability
severity: should_fix
---

**Rule.** When performing intermediate mathematical computations, accumulative
metric calculations, or multi-step transformations on array or buffer elements,
store intermediate values in local scalar variables. Avoid writing intermediate
values back into array or buffer elements when they are only consumed locally
within the loop or expression (avoiding dead stores), and avoid repeatedly
mutating destination memory across intermediate transformation steps before
assigning the final result.

**Why.** Writing intermediate values back to array or buffer memory creates
redundant store operations, dead stores, unnecessary type casts, and memory
aliasing obstacles that prevent compiler register allocation. Using local scalar
variables keeps intermediate calculations in registers, clarifies data flow, and
minimizes memory traffic by writing to destination memory at most once.

## Examples

Bad:

```cpp
// Redundant write to scratch array when value is only consumed locally:
for (idx = 1; idx < coeff_count; ++idx) {
  int64_t sqr_coeff = (int64_t)coeff[idx] * coeff[idx];
  coeff[idx] = (int16_t)((sqr_coeff * coeff[idx]) /
                         (sqr_coeff + (int64_t)median_val * median_val));
  wiener_variance += (int64_t)coeff[idx] * coeff[idx];
}

// Repeated intermediate mutations of destination buffer:
int v = clamp(pos[j] - blackclamp, 0, 255);
pos[j] = clamp(v - whiteclamp, 0, 255);
pos[j] += ref[j];
```

Good:

```cpp
// Use local scalar variable for accumulator calculation:
for (idx = 1; idx < coeff_count; ++idx) {
  int64_t sqr_coeff = (int64_t)coeff[idx] * coeff[idx];
  int64_t tmp_coeff = (sqr_coeff * coeff[idx]) /
                      (sqr_coeff + (int64_t)median_val * median_val);
  wiener_variance += tmp_coeff * tmp_coeff;
}

// Compute all steps in local variable and write to buffer once:
int v = clamp(pos[j] - blackclamp, 0, 255);
v = clamp(v - whiteclamp, 0, 255);
pos[j] = v + ref[j];
```
