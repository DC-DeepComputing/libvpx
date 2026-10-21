---
name: libvpx/vpx_ports/no-register-names-in-inline-asm-variables
rubric: correctness
severity: blocking
---

**Rule.** Do not name variables after CPU register identifiers (such as ax, bx,
cx, dx, eax, ebx, ecx, edx) when passing them into inline assembly or macros
expanding to inline assembly (such as cpuid). Use a distinctive prefix like
reg_eax or reg_ebx to avoid identifier collisions in MSVC 32-bit builds.

**Why.** In MSVC 32-bit x86 builds, __asm blocks reference C variable names
directly without qualification. When a variable name collides with a CPU
register, the inline assembler treats the symbol as the register rather than the
C variable, resulting in invalid assembly instructions or compilation errors.

## Examples

Bad:

```cpp
unsigned int ax, bx, cx, dx;
cpuid(0, 0, ax, bx, cx, dx);
```

Good:

```cpp
unsigned int reg_eax, reg_ebx, reg_ecx, reg_edx;
cpuid(0, 0, reg_eax, reg_ebx, reg_ecx, reg_edx);
```
