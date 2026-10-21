---
name: libvpx/valid-arm-neon-vector-type-suffix
rubric: correctness
severity: blocking
---

**Rule.** Always include the standard '_t' suffix when declaring ARM NEON vector
types (such as 'int32x4_t', 'uint8x16_t', or 'int64x2_t' rather than 'int32x4',
'uint8x16', or 'int64x2'). Pay particular attention to conditionally compiled
32-bit ARM fallback branches (such as '#else' under '#if VPX_ARCH_AARCH64' or
'#if defined(__aarch64__)') that are not exercised during 64-bit local builds.

**Why.** Standard ARM C Language Extensions (ACLE) specify vector type names
with a '_t' suffix. Omitting '_t' produces an invalid type name that goes
undetected when compiling exclusively on 64-bit ARM architectures, leading to
compilation failures in 32-bit ARM builds.

## Examples

Bad:

```cpp
#if VPX_ARCH_AARCH64
int64x2_t v_blocka[2];
#else
// Bad: missing '_t' suffix breaks 32-bit ARM builds.
int32x4 v_blocka[1];
#endif
```

Good:

```cpp
#if VPX_ARCH_AARCH64
int64x2_t v_blocka[2];
#else
// Good: standard ACLE type name with '_t' suffix.
int32x4_t v_blocka[1];
#endif
```
