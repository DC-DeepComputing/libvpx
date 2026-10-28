---
name: libvpx/separate-doxygen-comment-marker-from-text
description: When writing Doxygen documentation comments (`/*!` or `/**`) in header or source files, separate the opening comment delimiter from text with whitespace or a newline
rubric: readability
severity: nit
---

**Rule.** In Doxygen documentation comments (such as `/*!` or `/**`), separate
the opening delimiter from the comment text with whitespace (a space or newline)
or a leading Doxygen command. Do not abut plain comment text directly against
the opening `/*!` or `/**` marker without intervening whitespace.

**Why.** Placing comment text immediately adjacent to the opening Doxygen marker
degrades visual readability, violates codebase comment formatting conventions,
and risks parser inconsistencies across documentation generation tools.

## Examples

Bad:

```cpp
/*!Frame type for each frame in this GOP.
 * This will be populated to |update_type| in GF_GROUP defined in
 * vp9_firstpass.h
 */
vpx_rc_frame_update_type_t update_type[VPX_RC_MAX_STATIC_GF_GROUP_LENGTH + 2];
```

Good:

```cpp
/*!
 * Frame type for each frame in this GOP.
 * This will be populated to |update_type| in GF_GROUP defined in
 * vp9_firstpass.h
 */
vpx_rc_frame_update_type_t update_type[VPX_RC_MAX_STATIC_GF_GROUP_LENGTH + 2];
```
