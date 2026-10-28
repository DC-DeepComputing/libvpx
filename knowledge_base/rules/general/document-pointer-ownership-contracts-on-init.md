---
name: libvpx/document-pointer-ownership-contracts-on-init
description: When passing or receiving raw pointers in C interfaces, configuration structs, or initialization functions, document pointer ownership transfer conditions and freeing responsibilities
rubric: readability
severity: nit
---

**Rule.** When passing, adopting, or storing raw pointers across C interfaces,
configuration structs, or initialization functions, explicitly document the
memory ownership contract in comments. Specify whether ownership transfers to
the callee only upon successful initialization, which instance or caller is
responsible for freeing the allocated memory, and ensure failure paths release
or reset unowned pointers to prevent leaks or double-frees.

**Why.** In C codebases without RAII or smart pointers, ownership contracts
cannot be enforced by the type system. Ambiguous ownership semantics across
initialization functions, configuration structs, or multi-instance workflows
lead to memory leaks on failure paths or double-free vulnerabilities when both
caller and callee attempt to free resources.

## Examples

Bad:

```cpp
// Ambiguous ownership semantics on initialization and error paths
res = ctx->iface->init(ctx, &mr_cfg);
if (res != VPX_CODEC_OK) {
  return res;
}
```

Good:

```cpp
// ctx takes ownership of mr_cfg.mr_low_res_mode_info if and only if
// this call succeeds. The first ctx entry in the array is
// responsible for freeing the memory.
res = ctx->iface->init(ctx, &mr_cfg);
if (res != VPX_CODEC_OK) {
  // Release ownership on failure to prevent double-free
  priv->oxcf.mr_low_res_mode_info = NULL;
  return res;
}
```
