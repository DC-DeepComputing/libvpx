---
name: libvpx/document-input-file-format-above-parsing-logic
description: When parsing external input files or structured text configurations, document the expected file format and line schema in a comment above the parsing logic
rubric: readability
severity: nit
---

**Rule.** When reading or parsing external input files, test vectors, or text
configurations, document the expected file format, structure, and line schema in
an explanatory comment directly above the parsing logic (such as fscanf, sscanf,
or line-reading loops). Explicitly specify the expected fields, delimiter
syntax, and line structure so that readers and data creators know the required
input format without deciphering format specifiers.

**Why.** Input file formats and delimiter rules are difficult to discern from
format specifiers (such as %d,%d,%d,%d) or ad-hoc parsing routines alone.
Documenting the schema above the parser clarifies the expected specification for
maintainers and prevents malformed input data.

## Examples

Bad:

```cpp
// Open subtitle configuration file and read segments
fp = fopen(file_name, "r");
while (fscanf(fp, "%d,%d,%d,%d,%d,%d\n", &start_x, &start_y, &end_x, &end_y,
              &frame_start, &frame_end) == 6) {
  process_segment(start_x, start_y, end_x, end_y, frame_start, frame_end);
}
```

Good:

```cpp
// File format: repeated lines of "start_x,start_y,end_x,end_y,frame_start,frame_end\n"
fp = fopen(file_name, "r");
while (fscanf(fp, "%d,%d,%d,%d,%d,%d\n", &start_x, &start_y, &end_x, &end_y,
              &frame_start, &frame_end) == 6) {
  process_segment(start_x, start_y, end_x, end_y, frame_start, frame_end);
}
```
