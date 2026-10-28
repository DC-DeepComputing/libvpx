---
name: libvpx/pass-out-pointers-directly-to-callee
description: When forwarding output pointer parameters to a callee function that populates them, pass the out-pointer directly instead of using an intermediate local variable
rubric: readability
severity: nit
---

**Rule.** When an output pointer parameter received by a function is populated
by a callee or helper function, pass the out-pointer argument directly to the
callee. Avoid declaring intermediate local variables, passing their addresses to
the callee, and then copying the values to the dereferenced out-pointer.

**Why.** Passing the caller's out-pointer directly avoids unnecessary local
variable allocation, eliminates redundant assignment copying, and clarifies data
flow.

## Examples

Bad:

```cpp
void compute_block_stats(MACROBLOCK *x, int64_t *recon_error, int *eob) {
  int64_t this_recon_error = 0;
  int this_eob = 0;
  get_quantize_error(x, &this_recon_error, &this_eob);
  *recon_error = this_recon_error;
  *eob = this_eob;
}
```

Good:

```cpp
void compute_block_stats(MACROBLOCK *x, int64_t *recon_error, int *eob) {
  // Pass out-pointers directly to callee functions that populate them.
  get_quantize_error(x, recon_error, eob);
}
```
