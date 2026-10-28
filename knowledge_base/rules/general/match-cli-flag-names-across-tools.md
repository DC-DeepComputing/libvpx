---
name: libvpx/match-cli-flag-names-across-tools
description: When defining command-line arguments using ARG_DEF across CLI tools, keep option names consistent with existing binaries for shared features
rubric: readability
severity: should_fix
---

**Rule.** When adding or modifying command-line options using `ARG_DEF` or
`ARG_DEF_ENUM`, ensure flag names match existing conventions across related CLI
tools (such as `vpxenc`, `vpxdec`, and example utilities). For features or
settings already supported in another binary, reuse the established flag name
(e.g., `--row-mt` rather than inventing `--enable-row-mt`).

**Why.** Consistent command-line argument naming across tools keeps the CLI
interface intuitive, avoids fragmented flags for shared features, and prevents
user confusion across encoding and decoding workflows.

## Examples

Bad:

```cpp
static const arg_def_t rowmtarg =
    ARG_DEF(NULL, "enable-row-mt", 0, "Enable Multi-threading to run row-wise");
```

Good:

```cpp
static const arg_def_t rowmtarg =
    ARG_DEF(NULL, "row-mt", 1, "Enable multi-threading to run row-wise");
```
