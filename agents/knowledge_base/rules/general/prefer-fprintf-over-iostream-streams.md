---
name: libvpx/prefer-fprintf-over-iostream-streams
description: When printing diagnostic logs or error messages in C++ code, use standard C I/O like fprintf or printf instead of std::cerr or std::cout
rubric: readability
severity: should_fix
---

**Rule.** When outputting diagnostic messages, error logs, or debugging output
in C++ source files, use standard C I/O functions (`fprintf(stderr, ...)`,
`printf`) rather than C++ stream operators (`std::cerr`, `std::cout`,
`<iostream>`).

**Why.** Libvpx standardizes on standard C I/O across its codebase. Using
`<iostream>` and stream operators pulls in stream runtime machinery that
increases binary size, introduces static initializers, and triggers cpplint
`readability/streams` warnings.

## Examples

Bad:

```cpp
#include <iostream>

void LogFailure(const char* error_msg) {
  std::cerr << "init_encode_frame_result() failed: " << error_msg << "\n";
}
```

Good:

```cpp
#include <cstdio>

void LogFailure(const char* error_msg) {
  fprintf(stderr, "init_encode_frame_result() failed: %s\n", error_msg);
}
```
