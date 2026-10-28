---
name: libvpx/use-signed-types-for-negative-sentinels
description: When storing negative sentinel values like -1 to represent unassigned or invalid states, use signed integer types rather than unsigned types to avoid wraparound bugs
rubric: correctness
severity: should_fix
---

**Rule.** Use a signed integer type (such as int8_t, int16_t, or int) rather
than an unsigned integer type (such as uint8_t or unsigned int) for variables,
struct fields, or array elements that use negative sentinel values (such as -1)
to represent unassigned or invalid states. Do not initialize unsigned arrays or
variables to -1 (e.g., via memset(arr, -1, ...) or -1 assignments), as unsigned
values wrap around to positive numbers (such as 255 or UINT_MAX), breaking
sentinel assertions (like idx >= 0) and leading to out-of-bounds indexing.

**Why.** Initializing or assigning -1 to an unsigned integer causes wraparound
to positive values (e.g., 255 for uint8_t). This makes non-negative assertions
like assert(idx >= 0) tautologically true and breaks negative sentinel
comparisons, leading to silent corruption or out-of-bounds memory accesses.

## Examples

Bad:

```cpp
uint8_t recon_frame_index[REFS_PER_FRAME + MAX_ARF_LAYERS];
memset(recon_frame_index, -1, sizeof(recon_frame_index));
// ...
assert(recon_frame_index[i] >= 0);  // Unsigned is always >= 0; 255 passes!
cpi->tpl_recon_frames[i] = &frame_bufs[recon_frame_index[i]].buf;
```

Good:

```cpp
int8_t recon_frame_index[REFS_PER_FRAME + MAX_ARF_LAYERS];
memset(recon_frame_index, -1, sizeof(recon_frame_index));
// ...
assert(recon_frame_index[i] >= 0);
cpi->tpl_recon_frames[i] = &frame_bufs[recon_frame_index[i]].buf;
```
