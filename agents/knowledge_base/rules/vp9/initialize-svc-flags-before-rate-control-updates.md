---
name: libvpx/vp9/initialize-svc-flags-before-rate-control-updates
description: When initializing or updating VP9 encoder rate control contexts, set configuration mode flags such as cpi->use_svc before invoking rate control update routines.
rubric: correctness
severity: should_fix
---

**Rule.** In VP9 encoder and rate control initialization routines, configure
mode flags such as cpi->use_svc before calling rate control update or buffer
sizing functions (such as UpdateRateControl or vp9_rc_init). Ensure downstream
routines execute with the active SVC mode already initialized.

**Why.** Downstream rate control, layer context initialization, and buffer
sizing helpers check cpi->use_svc to branch on spatial and temporal layer
handling. Invoking update routines before setting use_svc causes multi-layer
rate control logic to be skipped or misconfigured.

## Examples

Bad:

```cpp
void VP9RateControlRTC::InitRateControl(const RateControlRTCConfig &rc_cfg) {
  // Bug: UpdateRateControl runs before use_svc is configured.
  UpdateRateControl(rc_cfg);

  cpi_->use_svc = (cpi_->svc.number_spatial_layers > 1 ||
                   cpi_->svc.number_temporal_layers > 1)
                      ? 1
                      : 0;
  vp9_rc_init(oxcf, 0, rc);
}
```

Good:

```cpp
void VP9RateControlRTC::InitRateControl(const RateControlRTCConfig &rc_cfg) {
  cpi_->use_svc = (cpi_->svc.number_spatial_layers > 1 ||
                   cpi_->svc.number_temporal_layers > 1)
                      ? 1
                      : 0;

  UpdateRateControl(rc_cfg);
  vp9_rc_init(oxcf, 0, rc);
}
```
