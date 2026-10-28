---
name: libvpx/vp9/worker-data-suffix-for-thread-context-structs
description: When defining per-worker multithreading context structures and fields, use the standard WorkerData suffix and _worker_data naming rather than generic Handle or Context suffixes
rubric: readability
severity: should_fix
---

**Rule.** Name per-worker threading context structures and typedefs with the
WorkerData suffix (such as RowMTWorkerData, TileWorkerData, LFWorkerData, or
EncWorkerData) and name corresponding member fields/variables with _worker_data
(or _data). Avoid inconsistent or generic suffixes like Handle or Context for
worker thread state.

**Why.** Libvpx multithreading pipelines (vp9 decoder, encoder, and loop-filter)
consistently pass worker payload structures suffixed with WorkerData to
VPxWorker hook callbacks. Standardizing on WorkerData clearly communicates
thread-private worker state and maintains uniform type conventions across
multithreaded subsystems.

## Examples

Bad:

```cpp
typedef struct RowMtHandle {
  int num_sbs;
  int *eob[MAX_MB_PLANE];
  PARTITION_TYPE *partition;
  tran_low_t *dqcoeff[MAX_MB_PLANE];
  int8_t *recon_map;
} RowMtHandle;

RowMtHandle *row_mt_handle;
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

RowMTWorkerData *row_mt_worker_data;
```
