---
name: libvpx/noun-or-file-suffix-for-path-cli-options
description: When defining CLI options in ARG_DEF that accept file paths or values, use nouns or '-file' suffixes rather than verb phrases
rubric: readability
severity: should_fix
---

**Rule.** When defining command-line options using ARG_DEF that accept file
paths or data values (has_arg = 1), name the option with a noun phrase or an
explicit suffix (such as '--tpl-stats-file') rather than a verb phrase (such as
'--write-tpl-stats'). Reserve verb-prefixed names for action commands or boolean
toggles.

**Why.** Verb-prefixed option names like '--write-...' sound like boolean
switches that enable a feature rather than options expecting a path or argument
value. Using nouns or explicit '-file' suffixes makes the expected argument type
immediately apparent to users and automated scripts.

## Examples

Bad:

```cpp
static const arg_def_t write_tpl_stats =
    ARG_DEF(NULL, "write-tpl-stats", 1, "Write TPL stats of vp9 encoder to file");
```

Good:

```cpp
static const arg_def_t tpl_stats_file =
    ARG_DEF(NULL, "tpl-stats-file", 1, "Write VP9 TPL stats to file");
```
