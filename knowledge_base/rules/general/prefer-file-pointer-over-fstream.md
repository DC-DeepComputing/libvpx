---
name: libvpx/prefer-file-pointer-over-fstream
description: When performing file I/O operations or passing file handles in APIs, prefer standard C file pointers (FILE*) and stdio over C++ fstreams (std::ofstream, std::ifstream)
rubric: readability
severity: should_fix
---

**Rule.** Prefer standard C file handles (FILE* or std::FILE*) and stdio
routines (<cstdio> / <stdio.h>) over C++ file stream types (std::ofstream,
std::ifstream, std::fstream) when declaring file I/O interfaces or performing
file input and output.

**Why.** libvpx is primarily a C codebase and consistently uses standard C file
pointers and stdio routines for file I/O across encoders, tools, examples, and
tests. Using FILE* maintains interoperability with C modules and avoids C++
stream overhead and header dependencies.

## Examples

Bad:

```cpp
#include <fstream>

void output_image_buffer(const ImageBuffer &image_buffer,
                         std::ofstream *out_file);
```

Good:

```cpp
#include <cstdio>

void output_image_buffer(const ImageBuffer &image_buffer,
                         FILE *out_file);
```
