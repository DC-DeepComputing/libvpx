---
name: libvpx/synchronize-comments-with-code-changes
description: When writing or modifying comments, docblocks, or preprocessor directives, keep referenced parameters, type aliases, conditions, formulas, vector widths, and macro names synchronized with code
rubric: readability
severity: should_fix
---

**Rule.** When adding, modifying, or refactoring documentation comments,
structured docblocks (`@param`, `Arguments:`, `Description:`), preprocessor
comments, or inline annotations, ensure that all descriptions accurately reflect
the underlying code:

1. **Parameters, Types, and Signatures**: Synchronize referenced function parameters (including `|param|` or `` `param` `` syntax), struct members, buffer variables, and type aliases (e.g., `typedef struct foo { ... } foo_t;`) with current declaration signatures rather than stale, copied, or pre-renamed identifiers.
2. **Logic, Conditions, and Preprocessor Guards**: When modifying variables, limits (such as `VPXMIN`/`VPXMAX` clamping), or conditional guards (`if`/`else if` branches, mode bypasses), update adjacent comments to match all active execution paths and thresholds. On preprocessor conditionals (`#if`, `#elif`, `#else`, `#endif`), update trailing comments to match current guard expressions and macro prefixes (e.g., `VPX_` rather than leftover `AOM_`).
3. **Operations, Formulas, and Algebraic Signs**: Accurately describe mathematical formulas, rate-distortion models, algebraic signs (+ vs. -), operator grouping, rounding vs. non-rounding shifts, and qualifiers (e.g., saturating vs. non-saturating arithmetic, load vs. store).
4. **SIMD Capacity and Architectural Context**: When porting routines across architectures or vector widths (e.g., SSE2/NEON to AVX2/AVX-512), update comments to match actual target vector widths and element counts (e.g., 16 16-bit elements for 256-bit AVX2 vs. 8 for 128-bit SSE2). Do not copy C reference implementation notes into architecture-specific SIMD/assembly files unless they apply, and omit redundant ISA comments on standard headers (e.g., use `#include <immintrin.h>` without `// AVX2`).
5. **Orphaned and Obsolete Comments**: Remove or relocate orphaned, duplicate, or obsolete comment blocks when deleting, moving, or refactoring code.

**Why.** Inaccurate, stale, or copy-pasted comments contradict the
implementation and mislead developers, reviewers, and automated tooling.
Mismatched parameter names or type aliases obscure API contracts and break doc
extraction tools. Inconsistent mathematical formulas or inverted algebraic signs
disguise subtle arithmetic and rate-control bugs. Stale preprocessor tags,
partial condition summaries, or obsolete SIMD element counts mask control flow
changes and confuse portable invariants with architectural optimizations.

## Examples

Bad:

```cpp
// 1. Stale parameter name and outdated type alias annotation:
// For the first call, |*bytes_in_buffer| should be 0.
int webm_read_frame(uint8_t **buffer, size_t *buffer_size);
typedef struct vpx_rational64 { int64_t num; int den; } vpx_rational64_t;  // alias for struct vpx_rational

// 2. Inconsistent mathematical formula / inverted algebraic sign:
// The model predicts bits: target_bits = bias + ratio * log2(q_step)
const double q_step = pow(2.0, (rq_model->bias - target_bits) / rq_model->ratio);

// 3. Stale preprocessor comment and incomplete condition comment:
#if (VPX_ARCH_X86 || VPX_ARCH_X86_64) && HAVE_MMX
void vpx_clear_system_state(void);
#endif  // VPX_ARCH_X86 && HAVE_MMX
// Key frame is set for very first frame or key frame flag.
if ((cm->current_video_frame == 0 && !svc->previous_frame_is_intra_only) ||
    (cpi->frame_flags & FRAMEFLAGS_KEY) ||
    (cpi->oxcf.mode != cpi->deadline_mode_previous_frame)) { ... }

// 4. Redundant ISA header comment and stale SSE2 element count on AVX2:
#include <immintrin.h>  // AVX2
// Multiply and narrow (load 8 16-bit values).
static INLINE __m256i mul_narrow(__m256i a) { ... }
```

Good:

```cpp
// 1. Synchronized parameter name and accurate type alias annotation:
// For the first call, |*buffer_size| should be 0.
int webm_read_frame(uint8_t **buffer, size_t *buffer_size);
typedef struct vpx_rational64 { int64_t num; int den; } vpx_rational64_t;  // alias for struct vpx_rational64

// 2. Consistent mathematical formula matching calculation:
// The model predicts bits: target_bits = bias - ratio * log2(q_step)
const double q_step = pow(2.0, (rq_model->bias - target_bits) / rq_model->ratio);

// 3. Synchronized preprocessor comment and complete condition comment:
#if (VPX_ARCH_X86 || VPX_ARCH_X86_64) && HAVE_MMX
void vpx_clear_system_state(void);
#endif  // (VPX_ARCH_X86 || VPX_ARCH_X86_64) && HAVE_MMX
// Key frame is set for first frame, key frame flag, or deadline mode change.
if ((cm->current_video_frame == 0 && !svc->previous_frame_is_intra_only) ||
    (cpi->frame_flags & FRAMEFLAGS_KEY) ||
    (cpi->oxcf.mode != cpi->deadline_mode_previous_frame)) { ... }

// 4. Clean header include and accurate AVX2 element count:
#include <immintrin.h>
// Multiply and narrow (load 16 16-bit values).
static INLINE __m256i mul_narrow(__m256i a) { ... }
```
