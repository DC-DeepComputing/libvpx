---
name: libvpx/descriptive-function-variant-names
description: When introducing or renaming function variants, use descriptive names indicating distinguishing inputs or behavior instead of vague suffixes like _simple or _2
rubric: readability
severity: should_fix
---

**Rule.** Do not use vague or uninformative suffixes such as `_simple`,
`_helper`, `_impl`, or numeric suffixes like `_2` when introducing or declaring
function variants. Use descriptive names indicating the specific inputs,
configuration parameters, or distinct behaviors that differentiate the variant
(e.g., `_with_video_info`).

**Why.** Vague suffixes fail to communicate to callers how a function variant
differs in contract, behavior, or required parameters compared to the base
version. Descriptive identifiers make API semantics self-documenting and prevent
confusion at call sites.

## Examples

Bad:

```cpp
// Vague suffix fails to explain what distinguishes this variant.
void ivf_write_file_header_simple(FILE *outfile, unsigned int fourcc,
                                  int frame_cnt, int frame_width,
                                  int frame_height, vpx_rational_t timebase);
```

Good:

```cpp
// Descriptive suffix clarifies the specific parameters and configuration provided.
void ivf_write_file_header_with_video_info(FILE *outfile, unsigned int fourcc,
                                           int frame_cnt, int frame_width,
                                           int frame_height,
                                           vpx_rational_t timebase);
```
