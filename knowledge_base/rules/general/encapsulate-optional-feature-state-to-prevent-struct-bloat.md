---
name: libvpx/encapsulate-optional-feature-state-to-prevent-struct-bloat
description: When adding optional or mode-specific fields to codec context structs, encapsulate them in a dedicated struct and allocate conditionally to avoid struct bloat
rubric: safety
severity: should_fix
---

**Rule.** When adding fields for optional modes, multithreading schemes, or
auxiliary features (such as row-based multithreading, tile synchronization, or
denoising) to core codec context structs (such as VP9Decoder, VP9_COMP, or
VP8D_COMP), encapsulate the feature state in a dedicated struct or handle.
Allocate the dedicated struct conditionally on demand when the feature is
enabled rather than inlining multiple feature-specific fields directly into the
main context struct.

**Why.** Inlining feature-specific fields into primary codec structs increases
the base memory footprint and object layout size for all decoder or encoder
instances, even when the feature is never enabled. Encapsulating optional state
into a dedicated struct allocated only when active keeps core context structs
lean and groups related feature data together.

## Examples

Bad:

```cpp
typedef struct VP9Decoder {
  VP9_COMMON common;
  // ...
  int row_mt;
  int num_sbs;
  int *eob[MAX_MB_PLANE];
  PARTITION_TYPE *partition;
  tran_low_t *dqcoeff[MAX_MB_PLANE];
  int8_t *recon_map;
} VP9Decoder;
```

Good:

```cpp
typedef struct RowMTWorkerData {
  int num_sbs;
  int *eob[MAX_MB_PLANE];
  PARTITION_TYPE *partition;
  tran_low_t *dqcoeff[MAX_MB_PLANE];
  int8_t *recon_map;
} RowMTWorkerData;

typedef struct VP9Decoder {
  VP9_COMMON common;
  // ...
  int row_mt;
  RowMTWorkerData *row_mt_worker_data;
} VP9Decoder;
```
