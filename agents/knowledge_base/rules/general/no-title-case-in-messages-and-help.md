---
name: libvpx/no-title-case-in-messages-and-help
description: When writing error messages, diagnostic logs, or CLI argument help strings, keep common nouns and descriptive technical terms lowercase rather than Title Case
rubric: readability
severity: nit
---

**Rule.** In error messages, diagnostic logs, and CLI argument descriptions, do
not title-case common nouns or descriptive feature terms (such as
multi-threading, row multi-thread mode, or loopfilter mode). Keep all non-proper
nouns lowercase, capitalizing only proper names, acronyms, or the initial word
of a sentence.

**Why.** Descriptive technical terms and feature options are common nouns rather
than proper nouns. Title-casing them in user-facing diagnostics and CLI help
strings creates grammatically inconsistent output across CLI tools and
diagnostics.

## Examples

Bad:

```cpp
fprintf(stderr, "Failed to set decoder in Row Multi-Thread Mode: %s\n",
        vpx_codec_error(&decoder));
static const arg_def_t row_mt =
    ARG_DEF(NULL, "row-mt", 1, "Enable Row Based Multi-Threading Mode");
```

Good:

```cpp
fprintf(stderr, "Failed to set decoder in row multi-thread mode: %s\n",
        vpx_codec_error(&decoder));
static const arg_def_t row_mt =
    ARG_DEF(NULL, "row-mt", 1, "Enable row based multi-threading mode");
```
