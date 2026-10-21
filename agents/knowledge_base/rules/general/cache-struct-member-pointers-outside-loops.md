---
name: libvpx/cache-struct-member-pointers-outside-loops
rubric: safety
severity: should_fix
---

**Rule.** Cache struct member pointers, array fields, and invariant struct
members in local const variables outside performance-critical loops rather than
dereferencing struct pointer members on every iteration. Avoid repeated
struct_ptr->member access in loops that perform memory writes.

**Why.** Compilers cannot always prove that memory writes inside a loop (such as
output buffer stores) do not alias struct contents, even when struct pointers
are const-qualified. Dereferencing struct members on every iteration prevents
loop-invariant code motion and forces compilers to emit redundant pointer
reloads and address calculations.

## Examples

Bad:

```cpp
for (i = 0; i < idx; i++) {
  abs_coeff += ROUND_POWER_OF_TWO(mb_plane->round[rc != 0], 1);
  tmp = ((((abs_coeff * mb_plane->quant[rc != 0]) >> 16) + abs_coeff) *
         mb_plane->quant_shift[rc != 0]) >>
        15;
  qcoeff_ptr[rc] = (tmp ^ coeff_sign) - coeff_sign;
}
```

Good:

```cpp
const int16_t *round_ptr = mb_plane->round;
const int16_t *quant_ptr = mb_plane->quant;
const int16_t *quant_shift_ptr = mb_plane->quant_shift;

for (i = 0; i < idx; i++) {
  abs_coeff += ROUND_POWER_OF_TWO(round_ptr[rc != 0], 1);
  tmp = ((((abs_coeff * quant_ptr[rc != 0]) >> 16) + abs_coeff) *
         quant_shift_ptr[rc != 0]) >>
        15;
  qcoeff_ptr[rc] = (tmp ^ coeff_sign) - coeff_sign;
}
```
