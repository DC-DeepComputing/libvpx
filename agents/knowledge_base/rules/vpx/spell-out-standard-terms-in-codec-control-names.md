---
name: libvpx/vpx/spell-out-standard-terms-in-codec-control-names
description: When defining public codec control enum identifiers and macros in vpx/ headers, spell out standard terms like LOOP_FILTER rather than abbreviated acronyms like LPF
rubric: readability
severity: should_fix
---

**Rule.** When defining public codec control enum IDs, type-binding macros
(VPX_CTRL_USE_TYPE), or header defines in vpx/ public headers, spell out
standard codec terms (such as LOOP_FILTER instead of LPF) consistently with
existing control naming conventions. Avoid ad-hoc or non-standard abbreviations
in public API control identifiers.

**Why.** Public codec controls form libvpx's stable external interface. Using
consistent, unabbreviated terminology across public control identifiers makes
the API predictable, intuitive, and searchable for downstream developers.

## Examples

Bad:

```cpp
// Using non-standard abbreviation LPF in public control identifier
VP9D_SET_LPF_OPT,
VPX_CTRL_USE_TYPE(VP9D_SET_LPF_OPT, int)
#define VPX_CTRL_VP9_SET_LPF_OPT
```

Good:

```cpp
// Spelling out standard term LOOP_FILTER to match existing controls
VP9D_SET_LOOP_FILTER_OPT,
VPX_CTRL_USE_TYPE(VP9D_SET_LOOP_FILTER_OPT, int)
#define VPX_CTRL_VP9_SET_LOOP_FILTER_OPT
```
