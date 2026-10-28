---
name: libvpx/avoid-redundant-dead-stores
description: When declaring or assigning local variables, struct fields, or SIMD vectors, eliminate redundant initializations, consecutive assignments, and dead stores overwritten before read
rubric: readability
severity: should_fix
---

**Rule.** Do not leave redundant initializations, consecutive assignments, dead
stores, or unused intermediate computations when assigning local variables or
struct fields. Eliminate redundant scalar initializations (such as '= 0', '=
0.0f', or '= NULL') and SIMD vector initializations (such as 'vmov_n_*',
'vdup_n_*', or zero-filling helpers) when the variable is unconditionally
assigned, computed, or fully populated by lane loads (such as 'vld1_lane_*' or
'vset_lane_*') before read. When configuring struct fields (such as 'oxcf',
'cm', 'cfg', or 'gf_group'), eliminate hardcoded default assignments immediately
overwritten by incoming configuration values, and remove dead intermediate or
SIMD calculations whose results are overwritten or never read.

**Why.** Redundant initializations, consecutive assignments, and dead stores
emit superfluous register-clearing instructions or wasted computation, clutter
data flow and initialization sequences, mislead readers about which defaults or
values are actively in effect, obscure SIMD/DSP pipeline dependencies, and
trigger static analyzer warnings (such as Coverity).

## Examples

Bad:

```cpp
// Variant 1: Redundant scalar or vector zero-initialization before overwrite or lane loads
int32_t sum_sq = 0;
sum_sq = HADD_SW_S32(src_r_w) + 16;

uint32x2_t d0 = vdup_n_u32(0);
d0 = vld1_lane_u32((const uint32_t *)dest0, d0, 0);

// Variant 2: Hardcoded struct defaults or fields immediately overwritten
oxcf->rc_mode = VPX_CBR;
oxcf->profile = cm->profile;
oxcf->rc_mode = rc_cfg.rc_mode;

gf_group->max_layer_depth = MAX_ARF_LAYERS - 1;
gf_group->max_layer_depth = 2;

// Variant 3: Dead intermediate computation immediately overwritten
s1_14 = vqaddq_s16(s2_13, s2_14);
s1_14 = vqsubq_s16(s2_12, s2_15);
```

Good:

```cpp
// Variant 1: Initialize directly at declaration or leave uninitialized for lane loads
int32_t sum_sq = HADD_SW_S32(src_r_w) + 16;

uint32x2_t d0;
d0 = vld1_lane_u32((const uint32_t *)dest0, d0, 0);

// Variant 2: Retain only the active configuration assignments
oxcf->profile = cm->profile;
oxcf->rc_mode = rc_cfg.rc_mode;

gf_group->max_layer_depth = 2;

// Variant 3: Eliminate dead computation and retain only used calculation
s1_14 = vqsubq_s16(s2_12, s2_15);
```
