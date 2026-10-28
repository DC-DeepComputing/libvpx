---
name: libvpx/avoid-duplicating-driver-and-loop-functions
description: When implementing processing variants or alternative execution modes, avoid duplicating driver routines or initialization logic; unify paths or extract shared setup helpers
rubric: design
severity: should_fix
---

**Rule.** Do not duplicate entire driver, decoding, or iteration functions when
only an inner sub-step differs, and avoid duplicating thread worker setup or
context initialization across alternative execution modes. Unify variant
processing paths within existing routines using internal conditional branches,
or factor shared initialization, synchronization, and loop control into common
helper functions.

**Why.** Cloning complex driver routines or duplicating worker allocation and
context resets causes code bloat, creates maintenance overhead, and risks logic
drift or desynchronization when initialization or synchronization requirements
change.

## Examples

Bad:

```cpp
// Variant 1: Duplicating entire driver/loop function for a minor inner step:
static const uint8_t *decode_tiles_split_parse_recon(VP9Decoder *pbi, ...) {
  // ... identical tile setup, loopfilter, and worker loops ...
  parse_partition(...);
  recon_partition(...);
}

// Variant 2: Duplicating multithreading initialization across MT routines:
const uint8_t *decode_tiles_row_wise_mt(VP9Decoder *pbi, ...) {
  if (pbi->num_tile_workers == 0) {
    // duplicate worker allocation and context resets...
  }
}
const uint8_t *decode_tiles_mt(VP9Decoder *pbi, ...) {
  if (pbi->num_tile_workers == 0) {
    // duplicate worker allocation and context resets...
  }
}
```

Good:

```cpp
// Variant 1: Handling variant inside existing shared loop:
static const uint8_t *decode_tiles(VP9Decoder *pbi, ...) {
  if (pbi->row_mt == 1) {
    parse_partition(...);
    recon_partition(...);
  } else {
    decode_partition(...);
  }
}

// Variant 2: Extracting shared worker/context initialization helper:
static INLINE void init_mt(VP9Decoder *pbi) {
  if (pbi->num_tile_workers == 0) {
    // allocate workers, init loopfilter, reset context...
  }
}
const uint8_t *decode_tiles_row_wise_mt(VP9Decoder *pbi, ...) {
  init_mt(pbi);
  ...
}
const uint8_t *decode_tiles_mt(VP9Decoder *pbi, ...) {
  init_mt(pbi);
  ...
}
```
