---
name: libvpx/name-variables-after-referenced-enum-or-type
description: When declaring intermediate variables holding values for specific enum constants or frame types, name variables consistently after the referenced enum identifier.
rubric: readability
severity: nit
---

**Rule.** Name intermediate variables, constants, and calculated metrics that
correspond to specific enum constants or frame types consistently after the
referenced enum identifier (e.g., 'qdelta_arf_low' for 'GF_ARF_LOW'). Avoid
using ambiguous numbers, arbitrary layer indexes, or inconsistent aliases that
diverge from the enum constant's name.

**Why.** Deriving intermediate variable names directly from the underlying enum
constant prevents confusion, preserves clear traceability between constants and
derived metrics, and makes the purpose of the calculation immediately obvious
without having to inspect enum definitions.

## Examples

Bad:

```cpp
// Ambiguous aliases like layer_2 when computing values for GF_ARF_LOW
int qdelta_arf_layer_2 =
    vp9_frame_type_qdelta(cpi, GF_ARF_LOW, active_worst_quality_base);
int active_worst_quality_layer2_arf = VPXMAX(
    active_worst_quality_base + qdelta_arf_layer_2, active_best_quality);
```

Good:

```cpp
// Variable names match the referenced enum constant (GF_ARF_LOW -> arf_low)
const int qdelta_arf_low =
    vp9_frame_type_qdelta(cpi, GF_ARF_LOW, active_worst_quality_base);
const int active_worst_quality_of_arf_low =
    VPXMAX(active_worst_quality_base + qdelta_arf_low, active_best_quality);
```
