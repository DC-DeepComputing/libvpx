---
name: libvpx/vp9/group-coupled-speed-features-into-control-structs
description: When defining feature controls in speed features or parameter structs, group mode and threshold parameters into dedicated control structs with explicit enums rather than loose fields or opposing flags
rubric: design
severity: should_fix
---

**Rule.** When adding, updating, or refactoring tightly coupled feature controls
or encoder configuration parameters (such as in SPEED_FEATURES or argument
structs like struct encode_b_args), group related algorithm modes and thresholds
into a dedicated control sub-structure (e.g., TRELLIS_OPT_CONTROL) with an
explicit enum typedef (e.g., ENABLE_TRELLIS_OPT_METHOD) rather than declaring
loose integer fields with magic level numbers. Additionally, do not use
redundant or opposing boolean flags (such as skip_* alongside enable_* or
disable_*) to control feature execution; consolidate execution control into a
single unified method enum or numeric threshold.

**Why.** Bundling tightly coupled parameters into dedicated control structs and
explicit enums clarifies algorithm mode and threshold interactions, eliminates
magic integer levels and ambiguous or contradictory flag combinations (such as
simultaneously enabling and skipping a feature), and prevents scattering related
configuration state across flat parameter structs.

## Examples

Bad:

```cpp
// Loose fields in SPEED_FEATURES with magic integer mode levels
typedef struct SPEED_FEATURES {
  int quant_coeff_opt_level;  // 0: disabled, 1: enabled, 2: src var
  double quant_opt_thresh;
} SPEED_FEATURES;

// Opposing boolean flags in encoder parameter structs
struct encode_b_args {
  MACROBLOCK *x;
  int enable_coeff_opt;
  int skip_coeff_opt;
  int *sse_calc_done;
};
```

Good:

```cpp
// Dedicated control struct with explicit enum constants in SPEED_FEATURES
typedef enum {
  DISABLE_TRELLIS_OPT,
  ENABLE_TRELLIS_OPT,
  ENABLE_TRELLIS_OPT_TX_RD_SRC_VAR,
} ENABLE_TRELLIS_OPT_METHOD;

typedef struct TRELLIS_OPT_CONTROL {
  ENABLE_TRELLIS_OPT_METHOD method;
  double thresh;
} TRELLIS_OPT_CONTROL;

typedef struct SPEED_FEATURES {
  TRELLIS_OPT_CONTROL trellis_opt_tx_rd;
} SPEED_FEATURES;

// Unified parameter struct using method / threshold controls
struct encode_b_args {
  MACROBLOCK *x;
  int enable_trellis_opt;
  double trellis_opt_thresh;
  int *sse_calc_done;
};
```
