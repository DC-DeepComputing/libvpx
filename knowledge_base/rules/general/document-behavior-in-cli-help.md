---
name: libvpx/document-behavior-in-cli-help
description: When defining CLI argument descriptions in ARG_DEF, use active phrasing with accurate ranges and format multi-choice options across indented lines without trailing punctuation
rubric: readability
severity: should_fix
---

**Rule.** When defining or modifying command-line argument help descriptions
using ARG_DEF or ARG_DEF_ENUM:
1. Use active phrasing stating the behavior or control provided (e.g., 'Control ...' or 'Enable ...' rather than bare noun phrases).
2. Accurately document supported parameter value ranges (including negative limits, defaults, and symmetric bounds) and highlight notable behavioral characteristics such as non-deterministic execution.
3. For multi-choice options, format distinct choices across separate lines using explicit newlines (\n) and aligned indentation, maintain parallel phrasing across items, and omit trailing semicolons, commas, or punctuation.

**Why.** Terminal --help usage output aligns and prints descriptions directly
without automatic word wrapping. Active phrasing clearly conveys the action
being configured, while accurate limits and execution notes prevent user
confusion. Formatting multi-choice items across indented lines with parallel
phrasing and omitting trailing delimiters eliminates visual clutter and makes
options easily scannable.

## Examples

Bad:

```cpp
// Bad: Bare noun phrase, unaligned multi-choice options with trailing semicolons.
static const arg_def_t disable_lf =
    ARG_DEF(NULL, "disable-loopfilter", 1,
            "Loopfilter in VP9\n"
            "0: Loopfilter on for all (default);\n"
            "1: Loopfilter off for non reference frames;\n"
            "2: Loopfilter off for all frames;");

// Bad: Compacting choices on a single line with trailing delimiters.
static const arg_def_t target_level = ARG_DEF(
    NULL, "target-level", 1,
    "Target level (255: off (default); 0: only keep level stats; 1: adaptively "
    "set alt-ref distance and tile column limit based on picture size);");

// Bad: Inaccurate parameter ranges or missing notable behavioral semantics.
static const arg_def_t cpu_used =
    ARG_DEF(NULL, "cpu-used", 1, "CPU Used (-8..9)");
static const arg_def_t row_mt =
    ARG_DEF(NULL, "row-mt", 1, "Enable row based multi-threading in VP9");
```

Good:

```cpp
// Good: Active verb phrasing, indented multi-choice lines with no trailing punctuation.
static const arg_def_t disable_lf =
    ARG_DEF(NULL, "disable-loopfilter", 1,
            "Control Loopfilter in VP9:\n"
            "                                        0: Loopfilter on for all frames (default)\n"
            "                                        1: Loopfilter off for non reference frames\n"
            "                                        2: Loopfilter off for all frames");

static const arg_def_t target_level = ARG_DEF(
    NULL, "target-level", 1,
    "Target level\n"
    "                                        255: off (default)\n"
    "                                          0: only keep level stats\n"
    "                                          1: adaptively set alt-ref "
    "distance and column tile limit based on picture size");

// Good: Accurate range and explicit documentation of non-deterministic behavior.
static const arg_def_t cpu_used =
    ARG_DEF(NULL, "cpu-used", 1, "CPU Used (-9..9)");
static const arg_def_t row_mt =
    ARG_DEF(NULL, "row-mt", 1,
            "Enable row based non-deterministic multi-threading in VP9");
```
