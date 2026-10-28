---
name: libvpx/name-static-factories-create-and-accept-config
description: When defining static factory methods that construct and return class instances, name them Create(...) rather than Init(...) and accept initialization config directly
rubric: readability
severity: should_fix
---

**Rule.** Name static factory methods that allocate and return new class
instances (such as std::unique_ptr<T>) Create(...) rather than Init(...). Pass
one-off initialization parameters and configuration structs directly into
Create(...) so returned instances are fully initialized, reserving Init(...) for
post-construction mutation methods on existing objects.

**Why.** In C++ interfaces, Create(...) clearly signals object instantiation and
ownership transfer, whereas Init(...) conventionally indicates initializing or
resetting state on an existing instance. Passing configuration into Create(...)
prevents exposing error-prone two-step initialization APIs that leave callers
with half-initialized objects.

## Examples

Bad:

```cpp
class VP9RateControlRTC {
 public:
  static std::unique_ptr<VP9RateControlRTC> Init();
  void InitRateControl(const RateControlRtcConfig &cfg);
};
```

Good:

```cpp
class VP9RateControlRTC {
 public:
  static std::unique_ptr<VP9RateControlRTC> Create(
      const RateControlRtcConfig &cfg);
 private:
  void InitRateControl(const RateControlRtcConfig &cfg);
};
```
