---
name: libvpx/declare-implicit-register-clobbers-in-inline-asm
description: When writing extended inline assembly blocks (asm / __asm__), declare all implicitly modified registers in the clobber list if not bound as output operands
rubric: correctness
severity: should_fix
---

**Rule.** In extended inline assembly blocks (`asm` / `__asm__`), explicitly
declare all registers implicitly modified by the assembly instructions in the
clobber list unless they are already bound as output operands. Do not omit
implicitly clobbered registers (such as `edx` in `rdtsc`/`rdtscp` or `ecx`/`edx`
in CPU feature queries) from the clobber list.

**Why.** Compilers assume inline assembly blocks leave any register not listed
in output operands or the clobber list untouched. Omitting implicitly modified
registers causes compiler register allocation conflicts, silent data corruption,
or segmentation faults.

## Examples

Bad:

```cpp
// rdtscp modifies edx implicitly, but edx is omitted from the clobber list.
__asm__ __volatile__("rdtscp\n\t" : "=a"(tscp) :);
```

Good:

```cpp
// Explicitly declare implicitly modified registers in the clobber list.
__asm__ __volatile__("rdtscp\n\t" : "=a"(tscp) : : "edx");
```
