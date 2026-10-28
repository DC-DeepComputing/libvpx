---
name: libvpx/pipe-delimiters-for-variable-references-in-comments
description: When referencing variable or parameter names in comments, enclose identifiers in pipe delimiters (|var|) rather than quotes or bare text
rubric: readability
severity: nit
---

**Rule.** Enclose variable and parameter names in pipe delimiters (e.g.,
`|var_name|`) when referencing them in code comments. Avoid using quotation
marks (such as `"var"` or `'var'`) or unadorned identifiers in explanatory
prose.

**Why.** Pipe delimiters (`|variable|`) distinguish code identifiers from
surrounding prose and align with libvpx comment conventions, enabling
code-indexing tools and human reviewers to reliably identify symbol references.

## Examples

Bad:

```cpp
/* "eob" will contain the index of the next found element */
// buffer holds the raw input bytes and stride is the row pitch.
```

Good:

```cpp
/* |eob| will contain the index of the next found element */
// |buffer| holds the raw input bytes and |stride| is the row pitch.
```
