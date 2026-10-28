---
name: libvpx/isolate-code-moves-from-style-cleanups
description: When structuring changes, isolate preparatory moves and refactorings into dedicated CLs, avoid drive-by cleanups, and do not add unused helpers ahead of time
rubric: readability
severity: should_fix
---

**Rule.** Keep CL scopes strictly focused and self-contained: isolate
preparatory code moves, helper extractions, and refactorings into dedicated
prerequisite 1:1 CLs before adding new feature logic, without bundling signature
changes or incidental style cleanups. When implementing features or
optimizations, do not include unrelated drive-by cleanups or parameter
refactorings in untouched functions, and do not introduce new helper functions,
macros, or definitions ahead of time before the commit or patch where they are
first needed.

**Why.** Mixing unrelated drive-by edits, opportunistic cleanups, or helper
extractions into a feature or code-move CL clutters diffs, complicates review,
muddies git blame history, and makes it harder to verify that no behavioral
regressions were introduced. Introducing unused helper functions ahead of time
clutters the patch, confuses reviewers searching for call sites, and risks
leaving dead code if subsequent changes are delayed or revised.

## Examples

Bad:

```cpp
// 1. Mixing unrelated drive-by cleanups in existing functions during a feature CL:
-static INLINE void highbd_add8x8(int16x8_t a0, int16x8_t a1, ...)
+static INLINE void highbd_add8x8(const int16x8_t a0, const int16x8_t a1, ...)

// 2. Bundling preparatory helper extraction with new feature logic in one CL:
-#include "vpx_dsp/arm/transpose_neon.h"
+#include "vpx_dsp/arm/idct_neon.h"
-static int16x8_t multiply_shift_and_narrow(...) { ... }
+// ... plus new feature implementation logic in the same CL ...

// 3. Modifying signatures or formatting during a pure code move CL:
int vpx_setup_noise(double sigma, char *noise, int size) {
  char_dist[next + j] = (char)i;  // Cleanup cast formatting during move
}

// 4. Introducing unused helper functions ahead of time before they are called:
// In a CL implementing 4x4 FDCT, adding a helper only needed for 8x8:
static INLINE int32x4_t shift_round_s32(int32x4_t x) { ... }
```

Good:

```cpp
// 1. Keep feature CLs focused on the feature; leave unrelated functions untouched:
static INLINE void highbd_add8x8(int16x8_t a0, int16x8_t a1, ...)
// Submit const/style cleanups in a dedicated follow-up CL.

// 2. Split preparatory refactoring into a preceding CL:
// CL 1 (pure move): Extract shared helpers to idct_neon.h without new feature logic.
// CL 2 (feature): Implement new optimization using idct_neon.h.

// 3. Keep code move CLs strictly 1:1, deferring signature or style adjustments:
int vpx_setup_noise(int size, double sigma, char *noise) {
  char_dist[next + j] = (char) (i);
}

// 4. Introduce new helpers in the patch where they are first needed:
// Omit shift_round_s32() from the 4x4 CL and add it in the subsequent 8x8 CL.
```
