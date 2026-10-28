---
name: libvpx/replace-unreachable-dead-branches-with-assertion
description: When disabling features or codepaths rendering branches unreachable, replace obsolete dead branch logic with assert(0 && "...") rather than leaving dead code
rubric: readability
severity: should_fix
---

**Rule.** When disabling a feature, mode, or codepath such that an execution
branch becomes unreachable, replace the obsolete dead branch body with an
assertion (such as assert(0 && "reason") or assert(0)) and an attributed TODO
for future cleanup rather than leaving dead implementation logic in place.

**Why.** Leaving obsolete implementation logic in branches rendered unreachable
by disabled feature flags or mode exclusions creates dead code, confuses
maintainers into believing the path is active, and increases maintenance
overhead. Replacing dead code with an explicit assertion clarifies invariants
and guards against regression.

## Examples

Bad:

```cpp
if (!args->cpi->sf.allow_skip_txfm_ac_dc &&
    skip_txfm_flag == SKIP_TXFM_AC_DC) {
  skip_txfm_flag = SKIP_TXFM_NONE;
}
if (skip_txfm_flag == SKIP_TXFM_NONE || ...) {
  ...
} else if (skip_txfm_flag == SKIP_TXFM_AC_ONLY) {
  ...
} else {
  // Obsolete dead branch handling SKIP_TXFM_AC_DC:
  x->plane[plane].eobs[block] = 0;
  sse = x->bsse[(plane << 2) + (block >> (tx_size << 1))] << 4;
  dist = sse;
}
```

Good:

```cpp
if (!args->cpi->sf.allow_skip_txfm_ac_dc &&
    skip_txfm_flag == SKIP_TXFM_AC_DC) {
  skip_txfm_flag = SKIP_TXFM_NONE;
}
if (skip_txfm_flag == SKIP_TXFM_NONE || ...) {
  ...
} else if (skip_txfm_flag == SKIP_TXFM_AC_ONLY) {
  ...
} else {
  // TODO(username): Remove this branch once the feature is fully cleaned up.
  assert(0 && "allow_skip_txfm_ac_dc does not allow SKIP_TXFM_AC_DC.");
}
```
