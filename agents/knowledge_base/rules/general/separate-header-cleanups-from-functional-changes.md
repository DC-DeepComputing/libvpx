---
name: libvpx/separate-header-cleanups-from-functional-changes
description: When modifying headers during functional changes, do not relocate existing public structs or bundle general header cleanups into unrelated feature CLs
rubric: change_scope
severity: should_fix
---

**Rule.** Do not relocate existing public structs, enums, or typedefs between
headers or bundle unrelated header cleanups and reorganizations into functional
feature or bug-fix CLs. Keep functional changes focused on their core behavior,
and isolate structural header refactoring and cleanup into separate, dedicated
commits.

**Why.** Mixing public struct relocations or general header cleanups into
functional CLs causes scope creep, obscures functional changes during review,
complicates git bisecting, and risks breaking downstream callers who include
specific headers.

## Examples

Bad:

```cpp
// In a functional CL updating SVC frame handling:
// Relocating vpx_svc_ref_frame_config from vpx/vp8cx.h to vpx/vpx_encoder.h
// along with reordering unrelated typedefs to clean up header organization.
```

Good:

```cpp
// In the functional CL: update vpx_svc_ref_frame_config in-place in vpx/vp8cx.h.
// In a separate dedicated CL: perform header reorganization or struct relocations.
```
