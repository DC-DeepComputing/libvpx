---
name: libvpx/prefer-public-setters-over-bloated-constructors
description: When adding optional or feature-specific configuration parameters to C++ classes, expose dedicated public member setters rather than expanding constructor signatures or using file-static helpers.
rubric: design
severity: should_fix
---

**Rule.** When adding optional, specialized, or feature-specific configuration
parameters to C++ API classes (such as SimpleEncode or rate control wrappers),
provide dedicated public setter/configuration member methods (or configuration
structs) instead of expanding constructor signatures with optional or default
arguments. Keep constructor parameter lists minimal and focused on mandatory
initialization parameters, and avoid implementing instance configuration via
file-static helper functions taking internal codec pointers (e.g. VP9_COMP
*cpi).

**Why.** Expanding constructor signatures with optional or evolving settings
breaks backwards compatibility for callers, clutters call sites with long lists
of fragile positional arguments, and hinders API extensibility. Exposing
dedicated public member methods provides a clean, self-documenting, and
maintainable interface for optional controls.

## Examples

Bad:

```cpp
// Bloating constructor with optional parameters and using static helpers:
static void SetExternalGroupOfPicture(VP9_COMP *cpi,
                                      const int *external_arf_indexes) {
  cpi->encode_command.use_external_arf = 1;
  cpi->encode_command.external_arf_indexes = external_arf_indexes;
}

SimpleEncode::SimpleEncode(int frame_width, int frame_height,
                           int frame_rate_num, int frame_rate_den,
                           int target_bitrate, int num_frames,
                           bool use_external_arf,
                           const int *external_arf_indexes = nullptr);
```

Good:

```cpp
// Exposing optional configuration via public member setter methods:
class SimpleEncode {
 public:
  SimpleEncode(int frame_width, int frame_height, int frame_rate_num,
               int frame_rate_den, int target_bitrate, int num_frames);

  void SetExternalGroupOfPicture(const int *external_arf_indexes);
  void SetExternalArfIndexes(const int *external_arf_indexes);
};
```
