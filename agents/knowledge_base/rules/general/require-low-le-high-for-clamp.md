---
name: libvpx/require-low-le-high-for-clamp
description: When replacing nested VPXMAX and VPXMIN with clamp(val, low, high), ensure low <= high is guaranteed across all execution paths
rubric: correctness
severity: should_fix
---

**Rule.** Do not replace nested `VPXMAX(low, VPXMIN(val, high))` expressions
with `clamp(val, low, high)` unless `low <= high` is guaranteed to hold under
all execution paths. If `high` can dynamically evaluate to less than `low`, use
nested `VPXMAX`/`VPXMIN` to preserve the unconditional lower bound floor.

**Why.** `clamp(val, low, high)` evaluates `val < low ? low : (val > high ? high
: val)` and returns `high` whenever `val > high`, violating `low` when `high <
low`. Nested `VPXMAX(low, VPXMIN(val, high))` strictly prioritizes `low` as an
unconditional minimum, preventing zero values and division-by-zero crashes.

## Examples

Bad:

```cpp
// If high_thresh can evaluate to <= 0, clamp() returns high_thresh (< 1)
// when max_bandwidth > high_thresh, causing division by zero downstream.
int max_rate = clamp(rc->max_frame_bandwidth, 1, high_thresh);
int frame_ratio = total_budget / max_rate;
```

Good:

```cpp
// VPXMAX unconditionally guarantees max_rate is at least 1 even if high_thresh <= 0.
int max_rate = VPXMAX(1, VPXMIN(rc->max_frame_bandwidth, high_thresh));
int frame_ratio = total_budget / max_rate;
```
