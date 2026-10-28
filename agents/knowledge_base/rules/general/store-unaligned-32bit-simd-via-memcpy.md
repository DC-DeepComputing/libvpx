---
name: libvpx/store-unaligned-32bit-simd-via-memcpy
description: When storing unaligned 4-byte (32-bit) SIMD vector data, extract to a 32-bit temporary and copy via memcpy or uint32_to_mem instead of lane-by-lane byte stores
rubric: safety
severity: should_fix
---

**Rule.** When storing unaligned 4-byte (32-bit) data from SIMD/NEON vectors to
memory, extract the data into a 32-bit integer temporary and copy it via memcpy
or memory store helpers (such as uint32_to_mem, store_u8_4x1, or
store_unaligned_u8q). Do not extract and write individual bytes lane-by-lane
with scalar lane accessors like vget_lane_u8.

**Why.** Extracting individual bytes lane-by-lane with scalar vector accessors
incurs significant instruction and throughput penalties. Reinterpreting and
extracting a 32-bit word into a temporary and copying via memcpy or inline store
helpers enables the compiler to emit an efficient unaligned scalar store without
risking alignment faults.

## Examples

Bad:

```cpp
// Bad: Writing 4 bytes lane-by-lane incurs heavy instruction overhead.
dst_ptr[0] = vget_lane_u8(a0, 0);
dst_ptr[1] = vget_lane_u8(a0, 1);
dst_ptr[2] = vget_lane_u8(a0, 2);
dst_ptr[3] = vget_lane_u8(a0, 3);
```

Good:

```cpp
// Good: Extract a 32-bit word and copy via memcpy or uint32_to_mem.
const uint32_t val = vget_lane_u32(vreinterpret_u32_u8(a0), 0);
memcpy(dst_ptr, &val, sizeof(val));

// Or use libvpx memory store helpers:
uint32_to_mem(dst_ptr, val);
```
