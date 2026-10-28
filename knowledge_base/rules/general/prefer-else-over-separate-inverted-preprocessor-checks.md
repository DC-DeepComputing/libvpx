---
name: libvpx/prefer-else-over-separate-inverted-preprocessor-checks
description: 'Consolidate adjacent and scattered same-configuration preprocessor guards into unified blocks, and use #if...#else with positive conditions instead of separate inverted checks'
rubric: readability
severity: should_fix
---

**Rule.** When structuring preprocessor conditionals, consolidate adjacent or
scattered configuration- and feature-dependent code (macros, helper functions,
constants, types, or tests sharing `#if CONFIG_...`, `ARCH_...`, or `HAVE_...`)
into a single unified conditional block rather than repeatedly reopening
isolated duplicate `#if` sections across the file. For mutually exclusive
branches, combine them into a single `#if ... #else` block instead of separate
inverted checks (such as `#if CONFIG_FOO` followed by `#if !CONFIG_FOO`), and
structure conditions with positive checks rather than leading negations (e.g.
`#if CONFIG_FOO` instead of `#if !CONFIG_FOO`). Additionally, annotate closing
`#endif` directives with a comment identifying the condition (e.g. `#endif //
CONFIG_FOO`).

**Why.** Closing and reopening identical preprocessor conditional guards in
sequence or scattering duplicate `#if CONFIG_...` blocks across a file creates
redundant boilerplate, clutters file structure, and fragments related helpers
and their callers. Consolidating identical guards and using unified `#if ...
#else` blocks with positive conditions clarifies build configurations, prevents
duplicate condition evaluations, and keeps configuration fallbacks clean and
maintainable.

## Examples

Bad:

```cpp
// Consecutive/adjacent duplicate preprocessor guards:
#if !CONFIG_REALTIME_ONLY
static unsigned int get_sby_perpixel_diff_variance(VP9_COMP *cpi, ...) { ... }
#endif

#if !CONFIG_REALTIME_ONLY
static void encode_rd_sb_row(VP9_COMP *cpi, ThreadData *td, ...) { ... }
#endif

// Separate inverted blocks and negated check:
#if CONFIG_OS_SUPPORT
typedef off_t FileOffset;
#endif
#if !CONFIG_OS_SUPPORT
typedef long FileOffset;
#endif

#if !CONFIG_REALTIME_ONLY
const int kCpuUsedVectors[] = { 1, 2, 3, 5, 6, 7 };
#else
const int kCpuUsedVectors[] = { 5, 6, 7 };
#endif
```

Good:

```cpp
// Consolidated into a single block with trailing comment:
#if !CONFIG_REALTIME_ONLY
static unsigned int get_sby_perpixel_diff_variance(VP9_COMP *cpi, ...) { ... }

static void encode_rd_sb_row(VP9_COMP *cpi, ThreadData *td, ...) { ... }
#endif  // !CONFIG_REALTIME_ONLY

// Unified #if...#else with positive conditions:
#if CONFIG_OS_SUPPORT
typedef off_t FileOffset;
#else
typedef long FileOffset;
#endif  // CONFIG_OS_SUPPORT

#if CONFIG_REALTIME_ONLY
const int kCpuUsedVectors[] = { 5, 6, 7 };
#else
const int kCpuUsedVectors[] = { 1, 2, 3, 5, 6, 7 };
#endif  // CONFIG_REALTIME_ONLY
```
