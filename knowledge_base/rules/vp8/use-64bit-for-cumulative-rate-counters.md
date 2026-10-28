---
name: libvpx/vp8/use-64bit-for-cumulative-rate-counters
description: Declare cumulative rate-control and frame-bit counters as int64_t consistently across encoder and layer contexts instead of 32-bit ints with ad-hoc clamping
rubric: correctness
severity: should_fix
---

**Rule.** When tracking cumulative bitrate, frame bit counts, or rate control
statistics (such as total_target_vs_actual, total_actual_bits,
total_target_bits, bits_off_target, vbr_bits_off_target, or total_byte_count)
across video frames, declare accumulator fields and variables as 64-bit signed
integers (int64_t) instead of 32-bit integers (int) with ad-hoc overflow
clamping. Additionally, ensure that accumulator fields are widened consistently
across all encoder contexts (VP8_COMP, VP9_COMP) and per-layer structures
(LAYER_CONTEXT, RATE_CONTROL).

**Why.** Cumulative bit differences and frame sizes across long video sequences
or high bitrates easily exceed 31-bit signed integer limits (INT_MAX, ~268 MB),
leading to integer overflow and undefined behavior. Ad-hoc clamping checks such
as 'accum >= INT_MAX - delta' are fragile and fail or trigger undefined behavior
when delta is signed or negative (e.g. rate undershoots). Furthermore, if
accumulator fields are widened in the primary encoder struct but left as 32-bit
integers in layer context structs, rate control calculations and debug logging
will silently truncate values or overflow during multi-layer temporal or spatial
encoding.

## Examples

Bad:

```cpp
// In vp8/encoder/onyx_int.h
typedef struct {
  int total_target_vs_actual;  // Left as 32-bit int in layer context
} LAYER_CONTEXT;

typedef struct VP8_COMP {
  int64_t total_target_vs_actual;
} VP8_COMP;

// Fragile clamping: fails or causes UB when delta is negative
if (lc->total_target_vs_actual >= INT_MAX - bits_off_for_this_layer)
  lc->total_target_vs_actual = INT_MAX;
else
  lc->total_target_vs_actual += bits_off_for_this_layer;
```

Good:

```cpp
// In vp8/encoder/onyx_int.h
typedef struct {
  int64_t total_target_vs_actual;  // Widened consistently across all contexts
} LAYER_CONTEXT;

typedef struct VP8_COMP {
  int64_t total_target_vs_actual;
} VP8_COMP;

// Direct 64-bit accumulation without fragile clamping
lc->total_target_vs_actual += bits_off_for_this_layer;
```
