---
name: libvpx/document-functions-at-declaration-not-call-sites
description: When documenting function contracts, place comments at header declarations or static definitions, specifying calling order, preconditions, and return-value semantics rather than at call sites
rubric: readability
severity: should_fix
---

**Rule.** Document a function's or method's interface, contract, general
purpose, preconditions, calling order, and return-value semantics—especially
non-obvious integer status codes, boolean flags, decision enums, or fallback
conventions (such as returning 0 for handled state and 1 for fallback)—at its
prototype declaration in the header file (for public/shared functions and class
APIs) or directly above its definition (for static helper routines). Clearly
specify what each return code signifies, expected caller fallback behavior, and
which setup or compute functions must run before dependent getters are called.
Additionally, document which output parameters or state queries are valid under
each outcome. Do not scatter interface contracts across call sites, duplicate
them at inline return statements, or bury them inside implementation bodies;
reserve comments inside bodies for non-obvious algorithm steps and call-site
comments for caller-specific rationale.

**Why.** Centralizing function contracts, lifecycle ordering, and return-value
semantics at header declarations or static definitions provides a single
authoritative source of truth for callers and maintainers. In C codebases like
libvpx and stateful C++ interfaces, functions often return status or decision
codes where 0 represents success/handled state and non-zero represents fallback
or error, or where getters depend on preceding compute steps. Placing interface
documentation inside implementation bodies hides contracts from header
consumers, while scattering or duplicating comments across call sites causes
visual clutter, misinterpretation of return values, and documentation divergence
when implementation details change. Undocumented calling order leads external
consumers to read uninitialized or stale state or invoke operations out of
sequence.

## Examples

Bad:

```cpp
// Variant 1: Interface contract buried inside implementation body instead of header
void vp9_check_reset_rc_flag(VP9_COMP *cpi) {
  // Check for resetting rc flags if avg_frame_bandwidth changes significantly.
  if (cpi->common.current_video_frame > 0) { ... }
}

// Variant 2: Undocumented return semantics or scattered at call sites
// Return 1 if no scale partitioning is done; variance partitioning used.
if (scale_partitioning_svc(cpi)) return;

static int scale_partitioning_svc(VP9_COMP *cpi, ...) {
  if (can_scale) {
    set_scaled_partition(...);
    return 0;
  }
  return 1;
}

// Variant 3: Precondition/rationale duplicated across multiple call sites
if (cpi->svc.simulcast_mode) non_reference_frame_simulcast(cpi);
...
if (cpi->svc.simulcast_mode) non_reference_frame_simulcast(cpi);

// Variant 4: Public API / class methods missing calling order and state validity
enum class FrameDropDecision { kOk, kDrop };
class VP9RateControlRTC {
 public:
  int GetQP() const;
  FrameDropDecision ComputeQP(const VP9FrameParamsQpRTC &frame_params);
  void PostEncodeUpdate(uint64_t encoded_frame_size);
};
```

Good:

```cpp
// Variant 1: Interface contract placed at header declaration
// In vp9_encoder.h:
// Check for resetting rc flags if avg_frame_bandwidth changes significantly.
void vp9_check_reset_rc_flag(VP9_COMP *cpi);

// Variant 2: Return semantics and fallback centralized at static definition
// Returns 0 if partition is set; returns 1 if no scale partitioning is done,
// signaling that variance partitioning should be used as fallback.
static int scale_partitioning_svc(VP9_COMP *cpi, ...) {
  if (can_scale) {
    set_scaled_partition(...);
    return 0;
  }
  return 1;
}

// Variant 3: Helper rationale centralized at static definition, not at call sites
// Never refresh reference frame buffers on top layers in simulcast mode.
static void non_reference_frame_simulcast(VP9_COMP *const cpi) { ... }
if (cpi->svc.simulcast_mode) non_reference_frame_simulcast(cpi);

// Variant 4: Public API calling order, return decisions, and validity documented
enum class FrameDropDecision { kOk, kDrop };
class VP9RateControlRTC {
 public:
  // GetQP() must be called after ComputeQP() to obtain the QP for the frame.
  int GetQP() const;
  // Computes QP if encoded (returns kOk). If dropped, returns kDrop and no QP
  // is computed (GetQP() and PostEncodeUpdate() must be skipped).
  FrameDropDecision ComputeQP(const VP9FrameParamsQpRTC &frame_params);
  // Updates rate control state after encoding; must be called after ComputeQP().
  void PostEncodeUpdate(uint64_t encoded_frame_size);
};
```
