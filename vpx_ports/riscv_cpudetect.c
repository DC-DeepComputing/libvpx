/*
 *  Copyright (c) 2026 The WebM project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#include "./vpx_config.h"
#include "vpx_ports/riscv.h"

#if CONFIG_RUNTIME_CPU_DETECT
#if defined(__riscv) && defined(__linux__)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/auxv.h>

#define RISCV_HWCAP_V (1 << ('v' - 'A'))

/* Some SoCs (e.g. SpacemiT K3 on older kernels) report AT_HWCAP without the
 * V bit even though the core supports RVV 1.0. Fall back to parsing the isa
 * string in /proc/cpuinfo, which is accurate on all kernels we target. */
static int cpuinfo_has_v(void) {
  FILE *f = fopen("/proc/cpuinfo", "r");
  char buf[4096];
  int has_v = 0;
  if (!f) return 0;
  {
    size_t n = fread(buf, 1, sizeof(buf) - 1, f);
    buf[n] = '\0';
    fclose(f);
  }
  {
    char *isa = strstr(buf, "isa");
    if (isa) {
      /* skip to ':' then scan tokens separated by '_' */
      while (*isa && *isa != ':') isa++;
      if (*isa == ':') {
        isa++;
        while (*isa) {
          if (*isa == '_') {
            isa++;
            if (isa[0] == 'v' && (isa[1] == '_' || isa[1] == '\n' ||
                                  isa[1] == '\0')) {
              has_v = 1;
              break;
            }
          } else if (*isa == ' ' || *isa == '\t' || *isa == '\n') {
            isa++;
          } else {
            isa++;
          }
        }
      }
    }
  }
  return has_v;
}

int riscv_cpu_caps(void) {
  int flags = 0;

  if ((getauxval(AT_HWCAP) & RISCV_HWCAP_V) || cpuinfo_has_v()) {
    flags |= HAS_RVV;
  }

  return flags;
}

#else  /* end __riscv && __linux__ */
#error                                                                       \
    "--enable-runtime-cpu-detect selected, but no CPU detection method " \
"available for your platform. Reconfigure with --disable-runtime-cpu-detect."
#endif
#else  /* end CONFIG_RUNTIME_CPU_DETECT */
int riscv_cpu_caps(void) { return 0; }
#endif
