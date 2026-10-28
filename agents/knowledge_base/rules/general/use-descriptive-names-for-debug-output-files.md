---
name: libvpx/use-descriptive-names-for-debug-output-files
description: When writing debug, experimental, or diagnostic dump files, use descriptive filenames reflecting the metric or subsystem instead of generic names like output.txt
rubric: readability
severity: nit
---

**Rule.** When creating or specifying filenames for debug, experimental, or
diagnostic dump files, use specific and descriptive names reflecting the exact
metric, feature, or subsystem being dumped (e.g.,
lowmotion_sum_of_motion_error.txt, framepsnr.stt). Avoid generic or ambiguous
placeholder names such as output.txt or dump.txt.

**Why.** Generic filenames like output.txt or dump.txt obscure what data or
metric was recorded, make multi-pass comparisons confusing, and risk accidental
file collisions or overwriting output from other tools and parallel runs.

## Examples

Bad:

```cpp
const char fname[] = "output.txt";
FILE *f = fopen(fname, "a");
if (f) {
  fprintf(f, "motion_error: %d\n", sum_err);
  fclose(f);
}
```

Good:

```cpp
const char fname[] = "lowmotion_sum_of_motion_error.txt";
FILE *f = fopen(fname, "a");
if (f) {
  fprintf(f, "motion_error: %d\n", sum_err);
  fclose(f);
}
```
