---
name: libvpx/mark-deprecated-api-and-struct-fields
description: When superseding API controls or struct fields while retaining them for backward compatibility, mark legacy declarations and handlers as deprecated with a TODO comment
rubric: readability
severity: nit
---

**Rule.** When extending or superseding API controls, configuration structures,
or struct fields while retaining legacy members for backward compatibility, mark
the legacy declarations, configuration parameters, and internal handlers with an
explicit deprecation TODO comment (e.g., '// TODO(owner): Deprecated, to be
removed.'). Clearly indicate that the legacy field is retained solely for
backward compatibility.

**Why.** Leaving superseded fields and handlers uncommented obscures API
evolution and causes ambiguity for both external users and internal maintainers.
Explicit deprecation comments document the planned retirement and prevent new
code from relying on obsolete mechanisms.

## Examples

Bad:

```cpp
// In public header / struct definition:
int temporal_layer_id;

// In codec control handler:
svc->temporal_layer_id = data->temporal_layer_id;
```

Good:

```cpp
// In public header / struct definition:
// TODO(username): Deprecated, to be removed.
int temporal_layer_id;

// In codec control handler:
// TODO(username): Deprecated, to be removed.
svc->temporal_layer_id = data->temporal_layer_id;
```
