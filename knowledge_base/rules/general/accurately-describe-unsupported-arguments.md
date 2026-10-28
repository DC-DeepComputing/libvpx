---
name: libvpx/accurately-describe-unsupported-arguments
description: When logging error messages for unsupported arguments or configurations, clearly state the supported options rather than claiming the input was invalid
rubric: readability
severity: nit
---

**Rule.** When reporting errors for unsupported arguments, configurations, or
container formats, clearly state the supported options or note that the feature
is unsupported rather than claiming the input was invalid or corrupt.

**Why.** Inaccurate error messages mislead users and developers into
troubleshooting file corruption or bad syntax when the actual issue is an
unsupported format or option.

## Examples

Bad:

```cpp
if (container == kContainerIVF) {
  // ...
  return writer;
}
fprintf(stderr, "VpxVideoWriter IVF container invalid.\n");
return NULL;
```

Good:

```cpp
if (container == kContainerIVF) {
  // ...
  return writer;
}
fprintf(stderr, "VpxVideoWriter supports only IVF.\n");
return NULL;
```
