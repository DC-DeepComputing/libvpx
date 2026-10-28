---
name: libvpx/omit-the-before-member-variables-in-comments
description: When referring to member variables or code identifiers in comments, use the identifier directly instead of prefixing it with 'the'
rubric: readability
severity: nit
---

**Rule.** When referencing member variables (such as those ending in a trailing
underscore) or specific code identifiers in comments, use the identifier
directly without prefixing it with the definite article 'the'. Refer to
variables by name or backticked symbol rather than phrasing them as English noun
phrases (e.g., write 'modify member_' rather than 'modify the member_').

**Why.** Member variables and code identifiers are already distinct and explicit
references. Prefixing them with 'the' in comment prose adds redundant
grammatical articles and creates awkward phrasing when referring to concrete
code symbols.

## Examples

Bad:

```cpp
// The following will check and modify the gop_map_ to make sure the
// gop_map_ satisfies the frame constraints.
void ValidateGopMap(GopMap* gop_map_) { ... }
```

Good:

```cpp
// The following will check and modify gop_map_ to make sure
// gop_map_ satisfies the frame constraints.
void ValidateGopMap(GopMap* gop_map_) { ... }
```
