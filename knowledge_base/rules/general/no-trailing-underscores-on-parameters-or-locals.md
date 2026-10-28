---
name: libvpx/no-trailing-underscores-on-parameters-or-locals
description: Omit trailing underscores from parameters, local variables, and C struct members; reserve trailing underscores exclusively for C++ class member variables.
rubric: readability
severity: should_fix
---

**Rule.** Do not use trailing underscores when naming function parameters, local
variables, or C struct fields and variables (e.g., use `recon_done_mutex`
instead of `recon_done_mutex_`). Reserve trailing underscores exclusively for
C++ class data members (`member_`), and use standard `snake_case` without
trailing underscores across all function arguments, local variables, and C
struct definitions.

**Why.** In Google and Chromium C++ style conventions, trailing underscores
uniquely distinguish class data members from local variables and function
parameters at every point of use. Adding trailing underscores to parameters or
locals creates misleading visual cues and obscures variable scope. In libvpx C
code, trailing underscores are non-idiomatic C++ artifacts (such as legacy
imports in `vpx_thread`) that violate standard C naming conventions and create
inconsistency across codec headers and implementations.

## Examples

Bad:

```cpp
// C++ function parameters and local variables
void ScaleFrame(unsigned int frame, bool flag_codec,
                bool smaller_width_larger_size_) {
  int local_count_ = 0;
  // ...
}

// C struct definitions and variables
struct VP9LfSyncData {
  pthread_mutex_t *recon_done_mutex_;
  pthread_cond_t *recon_done_cond_;
  int num_workers_;
};
```

Good:

```cpp
// C++ function parameters and local variables
void ScaleFrame(unsigned int frame, bool flag_codec,
                bool smaller_width_larger_size) {
  int local_count = 0;
  // ...
}

// C struct definitions and variables
struct VP9LfSyncData {
  pthread_mutex_t *recon_done_mutex;
  pthread_cond_t *recon_done_cond;
  int num_workers;
};
```
