---
name: libvpx/document-struct-field-scope-and-purpose
description: When declaring or modifying struct fields and configuration members, document their purpose, units, operational scope, state tracking, delta sign conventions, and buffer element count formulas
rubric: readability
severity: should_fix
---

**Rule.** When declaring or modifying struct fields, configuration parameters,
state variables, or API members in headers and structures, provide descriptive
doc comments detailing:
1. **Purpose and Semantics**: Describe what the field represents and clarify distinctions between similarly named members (such as external configuration vs. internal state).
2. **Units of Measurement**: Explicitly specify units for rates, sizes, durations, and coordinates (e.g., in kbps, ms, bytes, seconds, full-pixel vs. subpel units), and ensure related fields within the struct remain consistent.
3. **Operational Scope and Mode Restrictions**: State the applicable granularity (sequence-, frame-, layer-, or macroblock-level) and any mode, pass, or rate-control constraints (e.g., 1-pass VBR, 2-pass, CBR, SVC, RTC).
4. **State Tracking and Lifetimes**: For state flags, history tracking, inter-frame dependencies, and dynamic mode trackers, explain what state or transition behavior they record across frames or codec lifecycles.
5. **Dynamic Buffers and Dimension Trackers**: For dynamically allocated buffer pointers and their associated dimension, capacity, or size tracking members (e.g., `sb_rows`, `mi_rows`, `mi_cols`), document their relationship, the allocation state reflected by the tracking field, and the exact formula for deriving total element count and buffer capacity from related dimensions (such as frame dimensions or block sizes).
6. **Deltas and Offsets**: For fields representing deltas, offsets, or adjustments relative to a base value (e.g., `delta_q`, `delta_qp`, `uvdc_delta_q`), document the exact formula for how they combine with base values (whether added or subtracted) and explicitly specify their expected sign convention and valid range.

**Why.** In codec context and configuration structs, undocumented fields lead to
subtle bugs and API misuse:
- Omitting units (such as bps vs. kbps, or ms vs. seconds) causes configuration and rate-control errors.
- Omitting pass/mode constraints (such as 1-pass VBR or SVC) misleads callers into assuming restricted settings apply universally.
- Omitting state transition and lifetime details obscures inter-frame dependencies during maintenance.
- Omitting buffer dimension relationships and element count derivation formulas leaves iteration bounds and memory requirements ambiguous, risking buffer overflows, out-of-bounds indexing, and desynchronization between allocated capacity and tracking variables.
- Omitting delta combination formulas and sign conventions leads to inverted adjustments, sign errors, and corrupted quantization or motion vector baselines.

## Examples

Bad:

```cpp
// Variant 1: Undocumented units, mode restrictions, and state tracking
struct VP9RateControlRtcConfig {
  unsigned int ss_target_bitrate[VPX_SS_MAX_LAYERS];
  int max_consec_drop;
  int16_t mv_r;
  int last_frame_dropped;
};

// Variant 2: Undocumented buffer capacity derivation and dimension tracking
struct RD_OPT {
  int *row_base_thresh_freq_fact;
  int sb_rows;
};

// Variant 3: Undocumented delta combination formula and sign convention
struct UVDeltaQP {
  int uvdc_delta_q;
  int uvac_delta_q;
};
```

Good:

```cpp
// Variant 1: Scope, units, mode restrictions, and state transitions documented
struct VP9RateControlRtcConfig {
  // Target coding bitrate for each spatial layer in kbps.
  unsigned int ss_target_bitrate[VPX_SS_MAX_LAYERS];
  // Maximum consecutive dropped frames; only applies to SVC mode.
  int max_consec_drop;
  int16_t mv_r; /**< Motion vector row in pixel units */
  // Flag tracking whether the preceding frame was dropped during encoding.
  int last_frame_dropped;
};

// Variant 2: Dimension tracker documents allocation state and element count formula
struct RD_OPT {
  int *row_base_thresh_freq_fact;
  // Value of sb_rows when row_base_thresh_freq_fact was allocated.
  // The array has sb_rows * BLOCK_SIZES * MAX_MODES elements.
  int sb_rows;
};

// Variant 3: Delta fields document combination formula and sign convention
struct UVDeltaQP {
  // UV channel QP is GetQP() + uvdc/ac_delta_q, where delta values are negative.
  int uvdc_delta_q;
  int uvac_delta_q;
};
```
