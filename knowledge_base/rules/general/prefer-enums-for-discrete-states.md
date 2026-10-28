---
name: libvpx/prefer-enums-for-discrete-states
description: When declaring struct members, function parameters, variables, or return values representing discrete states, modes, or categories, use explicit enum types instead of raw integers or bools.
rubric: readability
severity: should_fix
---

**Rule.** Use explicit named enum types (such as `enum`, `typedef enum`, or
`enum class`) rather than raw integer types (`int`, `uint32_t`, etc.) or bare
boolean flags for struct fields, class members, local variables, function
parameters, and return types that represent discrete states, operational modes,
block sizes, or categories (e.g., `PREDICTION_MODE`, `TX_SIZE`, `BLOCK_SIZE`,
`SUBPEL_FORCE_STOP`, `RcFrameType`). Avoid documenting valid values solely
through comments with magic numbers or numeric ranges (e.g., `[0, 3]`). Ensure
both function signatures and call sites pass named enum constants to make caller
intent self-documenting. If a primitive integer is strictly required for
external C ABI compatibility, explicitly document valid values in a declaration
comment.

**Why.** Using raw integers or boolean literals discards compiler type safety,
obscures caller intent at call sites, and invites magic numbers or invalid state
assignments. Explicit enum types document domain semantics, provide
self-documenting code at call sites, enable compiler exhaustiveness checks in
switch statements, and prevent invalid out-of-range values at compile time.

## Examples

Bad:

```cpp
// BAD: Using raw integers with magic numeric ranges or comments
struct MotionVectorInfo {
  int ref_frame[2];  // Reference frame: -1 for none, [0, 3] for valid frame.
};

// BAD: Passing raw integers or boolean flags for modes/sizes
int vp8_compute_skin_block(const uint8_t *y, int y_stride, int bsize);
is_skin = vp8_compute_skin_block(y, y_stride, 0);

// BAD: Assigning enum-valued struct members to raw int
int subpel_force_stop = cpi->sf.mv.subpel_force_stop;
int tx_size = mbmi->tx_size;
```

Good:

```cpp
// GOOD: Dedicated enum type in struct field
enum RefFrameType { kNoneRefFrame = -1, kIntraFrame = 0, kLastFrame = 1, kGoldenFrame = 2, kAltRefFrame = 3 };
struct MotionVectorInfo {
  RefFrameType ref_frame[2];
};

// GOOD: Explicit named enum for parameter and call site
typedef enum SKIN_DETECTION_BLOCK_SIZE {
  SKIN_8X8,
  SKIN_16X16
} SKIN_DETECTION_BLOCK_SIZE;
int vp8_compute_skin_block(const uint8_t *y, int y_stride, SKIN_DETECTION_BLOCK_SIZE bsize);
is_skin = vp8_compute_skin_block(y, y_stride, SKIN_8X8);

// GOOD: Defined enum types for local variables
SUBPEL_FORCE_STOP subpel_force_stop = cpi->sf.mv.subpel_force_stop;
TX_SIZE tx_size = mbmi->tx_size;
```
