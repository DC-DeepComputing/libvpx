---
name: libvpx/prefer-vector-over-unique-ptr-array
description: When allocating or storing dynamically sized sequences of elements in C++ code, prefer std::vector<T> over std::unique_ptr<T[]> or raw dynamic arrays
rubric: readability
severity: should_fix
---

**Rule.** In C++ code, prefer `std::vector<T>` over `std::unique_ptr<T[]>` or
manually managed dynamic arrays for dynamically sized sequences of elements. Use
`std::vector` constructors or `.resize()` to allocate elements and manage their
lifetime and bounds.

**Why.** `std::vector<T>` automatically tracks its element count, provides safe
element access, memory management, and resizing, and integrates with standard
container algorithms. In contrast, `std::unique_ptr<T[]>` requires separate
manual tracking of the array length and lacks container conveniences.

## Examples

Bad:

```cpp
std::unique_ptr<PartitionInfo[]> partition_info =
    std::unique_ptr<PartitionInfo[]>(
        new PartitionInfo[num_rows_4x4 * num_cols_4x4]);
```

Good:

```cpp
std::vector<PartitionInfo> partition_info;
partition_info.resize(num_rows_4x4 * num_cols_4x4);
```
