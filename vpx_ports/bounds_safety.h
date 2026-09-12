/*
 *  Copyright (c) 2026 The WebM project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#ifndef VPX_VPX_PORTS_BOUNDS_SAFETY_H_
#define VPX_VPX_PORTS_BOUNDS_SAFETY_H_

/* Portability macros for optional Clang -fbounds-safety.
 *
 * When VPX_SUPPORT_FBOUNDS_SAFETY is defined (typically via
 * --enable-fbounds-safety and a Clang toolchain that implements
 * -fbounds-safety), these macros expand to Clang bounds annotations.
 * Otherwise they expand to nothing so default builds are unchanged.
 *
 * Pattern matches libwebp / libpng style: annotations are inert unless
 * explicitly enabled.
 */

#ifdef VPX_SUPPORT_FBOUNDS_SAFETY

#  include <ptrcheck.h>
/* Prefer __counted_by_or_null for pointers that may be NULL while the
 * companion size field is zero.
 */
#  define VPX_COUNTED_BY(n) __counted_by(n)
#  define VPX_COUNTED_BY_OR_NULL(n) __counted_by_or_null(n)

#else /* !VPX_SUPPORT_FBOUNDS_SAFETY */

#  define VPX_COUNTED_BY(n)
#  define VPX_COUNTED_BY_OR_NULL(n)

#endif /* VPX_SUPPORT_FBOUNDS_SAFETY */

#endif  // VPX_VPX_PORTS_BOUNDS_SAFETY_H_
