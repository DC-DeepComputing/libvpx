---
name: libvpx/guard-against-division-by-zero
description: When performing division or calculating ratios in rate control, estimation models, or metrics, guard denominators against zero or near-zero values before dividing.
rubric: correctness
severity: should_fix
---

**Rule.** When performing division, modulo, or ratio computations—particularly
in rate control, quantization, or analytical estimation models—verify that
denominators and divisors are non-zero (or use `fabs(denom) < epsilon` for
floating-point values) before dividing. Handle degenerate zero-divisor cases
explicitly with early returns, clamping, or safe fallback values instead of
allowing unchecked division.

**Why.** Dividing by zero causes integer arithmetic exceptions (SIGFPE) or
generates floating-point NaN/Inf values. Unchecked NaN and Inf values propagate
through rate-distortion models, quantization calculations, and encoder
heuristics, corrupting downstream frame encoding decisions and causing crashes
or undefined behavior.

## Examples

Bad:

```cpp
// Unchecked division in rate-distortion / quantization model
const double q_step =
    pow(2.0, (rq_model->bias - target_bits) / rq_model->ratio);
```

Good:

```cpp
// Guard against zero / near-zero divisor before division
if (fabs(rq_model->ratio) < 1e-6) {
  return -1;
}
const double q_step =
    pow(2.0, (rq_model->bias - target_bits) / rq_model->ratio);
```
