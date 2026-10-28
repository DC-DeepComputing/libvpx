---
name: libvpx/name-general-mechanisms-after-abstraction-not-use-case
description: When adding or modifying functions, variables, or comments for general-purpose features, name them after the underlying abstraction rather than a specific use case.
rubric: readability
severity: should_fix
---

**Rule.** Name functions, variables, types, files, and comments after the
general feature or abstraction (e.g., region of interest / `roi`, segmentation
map) rather than a single specific use case (e.g., `subtitles`) when the
underlying mechanism is not constrained to that application. Ensure identifier
names and documentation accurately reflect the general capability so the code
remains clear and reusable for other use cases.

**Why.** Tying identifiers and comments to a specific use case misleads readers
into thinking the mechanism is exclusive to that application, obscures the true
purpose of the abstraction, and causes confusion when the feature is reused for
other scenarios.

## Examples

Bad:

```cpp
// Setup and debug functions tied to subtitles use case:
int vp9_subtitles_setup(struct VP9_COMP *const cpi);
void vp9_subtitles_print_segmentation_map(struct VP9_COMP *const cpi);

// Variable named after a single application:
int subtitle_map_enabled;
```

Good:

```cpp
// General ROI mechanism named after the abstraction:
int vp9_roi_file_setup(struct VP9_COMP *const cpi);
void vp9_roi_file_print_segmentation_map(struct VP9_COMP *const cpi);

// Variable named after the general feature:
int roi_map_enabled;
```
