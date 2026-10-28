---
name: libvpx/document-canonical-struct-for-flattened-fields
description: When exposing structured internal fields or stats as flattened arrays or vectors, document the canonical struct and header defining field ordering.
rubric: readability
severity: nit
---

**Rule.** When declaring functions or API methods that expose structured data or
internal state as a flattened array or vector of raw values, include a
documentation comment referencing the canonical struct definition and header
file where the field ordering, types, and semantics are defined.

**Why.** Flattened numerical vectors or raw arrays obscure what individual
elements represent. Pointing callers to the canonical struct and its defining
header clarifies element layout and makes field mapping easy to understand and
maintain.

## Examples

Bad:

```cpp
// Returns the stats for each frame as a list of 25 double values.
std::vector<std::vector<double>> ObserveFirstPassStats();
```

Good:

```cpp
// Returns the stats for each frame as a list of 25 double values.
// For field definitions and ordering, see FIRSTPASS_STATS in vp9_firstpass.h.
std::vector<std::vector<double>> ObserveFirstPassStats();
```
