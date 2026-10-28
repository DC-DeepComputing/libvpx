---
name: libvpx/use-reset-to-reassign-unique-ptr
description: When reassigning an existing std::unique_ptr with a new pointer or resource, call .reset(...) directly instead of constructing and move-assigning a temporary std::unique_ptr
rubric: readability
severity: nit
---

**Rule.** To reassign an existing `std::unique_ptr` with a newly allocated
resource or raw pointer, call `.reset(...)` directly on the smart pointer rather
than constructing and move-assigning a temporary `std::unique_ptr` (e.g., `ptr =
std::unique_ptr<T>(...)` or `ptr = std::move(std::unique_ptr<T>(...))`).

**Why.** Calling `.reset(...)` directly is more concise and idiomatic C++. It
avoids redundant type repetition, unnecessary construction and destruction of
temporary `std::unique_ptr` objects, and superfluous `std::move` calls.

## Examples

Bad:

```cpp
image_buffer->plane_buffer[plane] =
    std::move(std::unique_ptr<uint8_t[]>(new uint8_t[w * h]));

ptr = std::unique_ptr<FrameDecoder>(new FrameDecoder());
```

Good:

```cpp
image_buffer->plane_buffer[plane].reset(new uint8_t[w * h]);

ptr.reset(new FrameDecoder());
```
