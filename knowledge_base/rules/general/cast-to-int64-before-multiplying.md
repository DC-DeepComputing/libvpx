---
name: libvpx/cast-to-int64-before-multiplying
description: When multiplying or scaling integer quantities exceeding 32 bits, cast a leading operand to int64_t/uint64_t and saturate at INT_MAX before downcasting to int
rubric: correctness
severity: should_fix
---

**Rule.** When multiplying integer operands or scaling rate/bandwidth quantities
whose intermediate calculation or product can exceed 32 bits (such as computing
data rates, bit budgets, frame bandwidth, dimensions, rate-distortion
multipliers, or event counts for probabilities), explicitly cast at least one
leading operand directly to int64_t or uint64_t before multiplication or
division. Do not cast parenthesized subexpressions (such as (int64_t)(a * b)),
do not perform intermediate math in 32-bit arithmetic, do not rely solely on
assignment to a 64-bit destination variable, and do not clamp probability counts
or rely on debug assertions to avoid overflow. Additionally, when downcasting or
assigning the 64-bit intermediate expression back to int (or a narrower integer
type), check for potential overflow and saturate or clamp at
INT_MAX/INT_MIN—either by bounding the input rate against its scaling factor
(e.g., VPXMIN(INT_MAX / scale, rate) or if (val > INT_MAX / factor) val =
INT_MAX;) or by clamping the evaluated 64-bit product with (int)VPXMIN(product,
(int64_t)INT_MAX)—and explicitly cast back to the destination type to satisfy
compiler narrowing conversion checks.

**Why.** In C, arithmetic operations on 32-bit operands are evaluated in 32-bit
precision before widening or assignment. Intermediate products of
high-resolution dimensions, framerates, rate-distortion multipliers, or
accumulated counts easily exceed INT_MAX or 32-bit limits before subsequent
division or right-shifting, producing undefined behavior from signed overflow
and silent data corruption. Directly casting the leading operand promotes all
subsequent intermediate math to 64-bit without distorting probability
distributions. Furthermore, even with 64-bit intermediate math, high bitrates or
scaling factors can produce final results that exceed INT_MAX; casting an
out-of-range 64-bit signed integer directly to int without saturation causes
signed integer wrap-around and corrupts rate control state. Clamping with VPXMIN
or factor-division checks and explicitly casting the final evaluated expression
prevents truncation bugs and satisfies compiler conversion checks (such as
-Wconversion or MSVC C4244).

## Examples

Bad:

```cpp
// Bad: Casting parenthesized subexpression evaluates multiplication in 32-bit first
const int64_t size =
    (int64_t)(cpi->oxcf.width * cpi->oxcf.height) * image_bps / 8;

// Bad: 32-bit intermediate product overflows before division
raw_target_rate = (oxcf->width * oxcf->height * 8) * 3 * framerate / 8000;

// Bad: Downcasting 64-bit intermediate product to int without saturation wraps on overflow
const int target = (int)((int64_t)rc->avg_frame_bandwidth * kf_ratio);
const int target_rate =
    (int)(((int64_t)cpi->rc.sb64_target_rate * xmis * ymis * 256) /
          (bw * bh));

// Bad: Clamping counts distorts probabilities; unpromoted product still risks overflow
const unsigned int tot = VPXMIN(c[0] + c[1], (1 << 24) - 1);
const unsigned int p = (((uint64_t)c[0] * Pfac) + (rd ? tot >> 1 : 0)) / tot;
```

Good:

```cpp
// Good: Cast leading operand directly to int64_t to evaluate math in 64-bit
const int64_t size =
    (int64_t)cpi->oxcf.width * cpi->oxcf.height * image_bps / 8;

// Promote to int64_t before multiplying/dividing and cast evaluated result
raw_target_rate = (unsigned int)(((int64_t)oxcf->width * oxcf->height *
                                  oxcf->bit_depth * 3) *
                                 oxcf->init_framerate / 8000);

// Good: Saturate at INT_MAX before multiplying or clamp 64-bit result before downcasting
int target = rc->avg_frame_bandwidth;
if (target > INT_MAX / kf_ratio) {
  target = INT_MAX;
} else {
  target = rc->avg_frame_bandwidth * kf_ratio;
}

cpi->rc.sb64_target_rate =
    VPXMIN(INT_MAX / 256, cpi->rc.sb64_target_rate);
const int target_rate =
    (int)(((int64_t)cpi->rc.sb64_target_rate * xmis * ymis * 256) /
          (bw * bh));

// Good: Cast count to uint64_t and explicitly cast evaluated expression to unsigned int
const unsigned int tot = c[0] + c[1];
const unsigned int p =
    (unsigned int)(((uint64_t)c[0] * Pfactor) + (Round ? tot >> 1 : 0)) / tot;
```
