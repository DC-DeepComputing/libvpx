---
name: libvpx/vpx_dsp/preserve-tran-high-for-transform-constants
description: When defining transform constants or intermediate calculation variables, use tran_high_t rather than narrower fixed-width types to prevent arithmetic overflow from improper type promotion
rubric: correctness
severity: should_fix
---

**Rule.** Do not narrow transform constant definitions (such as cospi_*_64 or
sinpi_*_9) or intermediate calculation variables from tran_high_t to narrower
fixed-width types like int16_t. Keep transform multipliers and intermediate
stages typed as tran_high_t so arithmetic expressions promote operands to the
required 32-bit or 64-bit precision across both standard and high-bitdepth
builds.

**Why.** In libvpx, tran_high_t expands from int32_t in standard builds to
int64_t when CONFIG_VP9_HIGHBITDEPTH is enabled. Narrowing transform constants
or intermediate values to int16_t causes arithmetic multiplications and
additions to evaluate with insufficient bit-width, risking silent integer
overflow and corrupted transform outputs.

## Examples

Bad:

```cpp
// Narrowing constants to int16_t prevents proper 32/64-bit promotion in transform math.
static const int16_t cospi_1_64 = 16364;
static const int16_t cospi_31_64 = 804;

void idct_stage(const int16_t *input, tran_high_t *output) {
  // Multiplications risk 16-bit intermediate overflow if constants are narrowed
  output[0] = input[0] * cospi_1_64 + input[1] * cospi_31_64;
}
```

Good:

```cpp
// Using tran_high_t ensures intermediate multiplications promote to 32-bit or 64-bit precision.
static const tran_high_t cospi_1_64 = 16364;
static const tran_high_t cospi_31_64 = 804;

void idct_stage(const int16_t *input, tran_high_t *output) {
  output[0] = input[0] * cospi_1_64 + input[1] * cospi_31_64;
}
```
