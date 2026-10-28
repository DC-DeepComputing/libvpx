---
name: libvpx/pass-statically-known-callbacks-as-arguments
description: When invoking callbacks statically known at call sites, pass function pointers directly as parameters instead of storing them in context structs
rubric: design
severity: should_fix
---

**Rule.** Pass callback function pointers directly as function parameters rather
than storing them in runtime context structs or lookup arrays when the caller
statically knows which function to invoke. Do not add function pointer fields,
dispatch tables, or placeholder dummy functions to context structs when dispatch
can be determined at call sites.

**Why.** Storing function pointers in runtime context structs when the callee is
known statically adds unnecessary state management, per-frame initialization
overhead, and dummy placeholder functions. Passing callbacks directly as
parameters simplifies data structures, clarifies call semantics, and enables
compiler optimizations.

## Examples

Bad:

```cpp
// Bad: Storing callbacks in runtime structs and dispatching via lookup arrays
pbi->row_mt_worker_data->process_block[0] = process_block_void;
pbi->row_mt_worker_data->process_block[PARSE] = parse_block;
pbi->row_mt_worker_data->process_block[RECON] = recon_block;

pbi->row_mt_worker_data->process_block[parse_recon_flag](twd, pbi, mi_row, mi_col);
```

Good:

```cpp
// Good: Pass callback function pointer directly as a parameter
static void process_partition(TileWorkerData *twd, VP9Decoder *const pbi,
                              int mi_row, int mi_col,
                              process_block_fn_t process_block) {
  process_block(twd, pbi, mi_row, mi_col);
}

// Call site statically passes the required function pointer
process_partition(twd, pbi, mi_row, mi_col, recon_block);
```
