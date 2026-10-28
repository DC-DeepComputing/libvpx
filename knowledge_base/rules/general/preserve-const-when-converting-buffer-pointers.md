---
name: libvpx/preserve-const-when-converting-buffer-pointers
description: When converting or casting read-only buffer pointers using macros like CONVERT_TO_SHORTPTR or CAST_TO_SHORTPTR, preserve const qualification on the destination pointer
rubric: readability
severity: should_fix
---

**Rule.** When converting or casting read-only buffer pointers using conversion
macros (such as CONVERT_TO_SHORTPTR, CAST_TO_SHORTPTR, CONVERT_TO_BYTEPTR, or
CAST_TO_BYTEPTR), declare the destination pointer with const qualification
(e.g., const uint16_t * or const uint8_t *). Do not drop const when assigning
the macro result to a local pointer variable.

**Why.** Because C-style cast macros cast unconditionally to non-const pointer
types (uint16_t * or uint8_t *), passing a const source pointer silently
discards its const qualifier. Assigning the output to a mutable pointer
undermines const-correctness and risks accidental mutation of read-only source
buffers.

## Examples

Bad:

```cpp
// a8 is 'const uint8_t *'
uint16_t *a = CONVERT_TO_SHORTPTR(a8);
uint8_t *b = CONVERT_TO_BYTEPTR(b16);
```

Good:

```cpp
// Destination pointer preserves const qualifier
const uint16_t *a = CONVERT_TO_SHORTPTR(a8);
const uint8_t *b = CONVERT_TO_BYTEPTR(b16);
```
