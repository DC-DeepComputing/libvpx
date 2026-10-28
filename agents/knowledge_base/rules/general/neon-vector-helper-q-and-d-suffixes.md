---
name: libvpx/neon-vector-helper-q-and-d-suffixes
description: When defining ARM NEON vector helper functions, use explicit 'q' and 'd' suffixes to balance 128-bit quadword and 64-bit doubleword operations
rubric: readability
severity: should_fix
---

**Rule.** When defining or updating ARM NEON vector helper functions (such as
load, store, or conversion helpers), use explicit 'q' and 'd' suffixes (e.g.,
load_tran_low_to_s16q vs. load_tran_low_to_s16d) to distinguish 128-bit quadword
and 64-bit doubleword variants. Do not leave the 128-bit version unsuffixed when
a 64-bit counterpart exists or vector widths differ.

**Why.** ARM NEON distinguishes 64-bit doublewords ('d' registers, e.g.
int16x4_t) from 128-bit quadwords ('q' registers, e.g. int16x8_t). Suffixing
both variants with balanced 'q' and 'd' tags makes vector register widths
unambiguous at call sites and matches NEON intrinsic naming conventions across
libvpx.

## Examples

Bad:

```cpp
// Ambiguous 128-bit quadword helper without 'q' suffix alongside 64-bit helper
static INLINE int16x8_t load_tran_low_to_s16(const tran_low_t *buf);
static INLINE int16x4_t load_tran_low_to_s16d(const tran_low_t *buf);

static INLINE void store_s16_to_tran_low(tran_low_t *buf, const int16x8_t a);
```

Good:

```cpp
// Explicit 'q' and 'd' suffixes distinguishing 128-bit and 64-bit vector widths
static INLINE int16x8_t load_tran_low_to_s16q(const tran_low_t *buf);
static INLINE int16x4_t load_tran_low_to_s16d(const tran_low_t *buf);

static INLINE void store_s16q_to_tran_low(tran_low_t *buf, const int16x8_t a);
```
