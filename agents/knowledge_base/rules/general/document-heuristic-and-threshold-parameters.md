---
name: libvpx/document-heuristic-and-threshold-parameters
description: When defining heuristic, ML, or speed feature threshold fields and parameters, document what values represent, special sentinels, and speed-versus-quality trade-offs
rubric: readability
severity: nit
---

**Rule.** When defining heuristic, machine-learning, speed-feature, or tuning
threshold parameters and struct fields, document what the values represent,
their units or expected range, how special sentinel values (such as 0 or
negative numbers) behave, and the directional speed-versus-quality trade-off
(such as whether higher values increase aggressiveness for encoding speed at the
cost of compression performance).

**Why.** Empirically tuned cutoffs and model confidence thresholds are opaque
without documentation, making it difficult for future maintainers to understand
their units, adjust tuning parameters safely, or predict whether changing values
favors encoding speed or compression efficiency.

## Examples

Bad:

```cpp
// Use ML-based partition search early breakout.
int use_ml_partition_search_breakout;
float ml_partition_search_breakout_thresh[3];
```

Good:

```cpp
// Use ML-based partition search early breakout.
int use_ml_partition_search_breakout;
// Early breakout thresholds for ML partition search across block sizes.
// Higher values increase breakout aggressiveness (improving encoding speed
// at the expense of compression quality). Sentinel <= 0 disables breakout.
float ml_partition_search_breakout_thresh[3];
```
