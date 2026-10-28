---
name: libvpx/initialize-local-variables-with-default-values
description: Initialize local variables to their default or baseline value at declaration and conditionally override them, avoiding redundant else branches and dead sentinel initializations
rubric: readability
severity: nit
---

**Rule.** Initialize local variables directly to their default or baseline
fallback value at declaration and conditionally override them when alternate
conditions hold (or use a ternary operator). Avoid declaring uninitialized
variables with redundant else branches that merely assign the default, and avoid
initializing to an unused sentinel or third state while branching separately
over binary choices (e.g., `if (choice == 0) ... else if (choice == 1) ...`).

**Why.** Pre-initializing variables directly to their default value simplifies
control flow, eliminates redundant else or else-if branches, avoids dead
initialization to unused sentinel states, reduces indentation, and ensures
variables are safely initialized.

## Examples

Bad:

```cpp
// Redundant else branch for default value
int num_sbs;
if (pbi->max_threads > 1) {
  num_sbs = sb_cols * sb_rows;
} else {
  num_sbs = 1;
}

// Dead initialization to unused third state for binary choice
vpx_enc_deadline_t deadline = VPX_DL_GOOD_QUALITY;
if (dl_choice == 0) {
  deadline = VPX_DL_REALTIME;
} else if (dl_choice == 1) {
  deadline = VPX_DL_BEST_QUALITY;
}
```

Good:

```cpp
// Direct default initialization with conditional override
int num_sbs = 1;
if (pbi->max_threads > 1) {
  num_sbs = sb_cols * sb_rows;
}

// Initialized to default choice and conditionally overridden
vpx_enc_deadline_t deadline = VPX_DL_REALTIME;
if (dl_choice == 1) deadline = VPX_DL_BEST_QUALITY;
```
