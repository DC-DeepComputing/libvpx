---
name: libvpx/vp9/encapsulate-optimization-decisions-in-helpers
description: When implementing encoder optimization heuristics, evaluate decision logic in dedicated helper functions rather than caching intermediate boolean flags in shared structures like MACROBLOCK
rubric: design
severity: should_fix
---

**Rule.** When implementing encoder speed features or optimization heuristics
(such as trellis quantization or transform skipping), encapsulate the decision
logic in a dedicated helper function (e.g. static INLINE int
do_trellis_opt(...)) rather than storing intermediate boolean decision flags in
shared structures like MACROBLOCK. Pass required metric arguments or compute the
condition locally where the decision is consumed.

**Why.** Caching transient decision flags across encoding stages adds
unnecessary fields to shared per-block structs like MACROBLOCK, causing struct
bloat and state scattering. Encapsulating the decision in a dedicated helper
function localizes speed-feature threshold checks and block metric evaluations,
keeping shared structs clean and decoupled.

## Examples

Bad:

```cpp
// Bad: Caching intermediate heuristic decision flag in MACROBLOCK struct.
x->block_qcoeff_opt =
    cpi->sf.quant_coeff_opt_level && (logvar <= cpi->sf.quant_opt_thresh);

// Later in transform/quantization routine:
const int enable_coeff_opt =
    x->block_qcoeff_opt && !skip_coeff_optimize(cpi, x);
```

Good:

```cpp
// Good: Encapsulating decision logic inside a dedicated static helper.
static INLINE int do_trellis_opt(const struct encode_b_args *args,
                                 const MACROBLOCK *x) {
  if (args->trellis_opt_thresh > 0.0) {
    return x->log_block_src_var <= args->trellis_opt_thresh;
  }
  return 1;
}

// Consuming the decision via the helper function:
const int enable_trellis_opt = do_trellis_opt(args, x);
```
