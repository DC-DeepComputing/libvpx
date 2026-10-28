---
name: libvpx/cli-toggle-flags-accept-argument
description: When adding boolean or feature toggle options using ARG_DEF, configure them to accept an argument (has_arg = 1) parsed with arg_parse_uint for explicit disablement
rubric: readability
severity: should_fix
---

**Rule.** When defining configurable boolean flags or feature toggle options in
CLI tools using ARG_DEF, set has_arg to 1 and parse the value using
arg_parse_uint(&arg) rather than defining bare 0-argument flags. This allows
scripts and users to explicitly toggle features on or off (e.g., --feature=0 vs
--feature=1) without conditionally omitting command-line arguments.

**Why.** Bare 0-argument boolean flags only allow enabling a feature when
present on the command line. Accepting an explicit numeric argument (0 or 1)
allows test runners, wrapper scripts, and automation to explicitly disable
features or pass dynamic configuration values without complex command-line
string manipulation.

## Examples

Bad:

```cpp
static const arg_def_t rowmtarg =
    ARG_DEF(NULL, "row-mt", 0, "Enable multi-threading to run row-wise in VP9");
// ...
} else if (arg_match(&arg, &rowmtarg, argi)) {
  enable_row_mt = 1;
}
```

Good:

```cpp
static const arg_def_t rowmtarg =
    ARG_DEF(NULL, "row-mt", 1, "Enable multi-threading to run row-wise in VP9");
// ...
} else if (arg_match(&arg, &rowmtarg, argi)) {
  enable_row_mt = arg_parse_uint(&arg);
}
```
