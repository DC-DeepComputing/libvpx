/*
 *  Copyright (c) 2010 The WebM project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#ifndef VPX_VP9_ENCODER_VP9_ENCODEMB_H_
#define VPX_VP9_ENCODER_VP9_ENCODEMB_H_

#include "./vpx_config.h"
#include "vp9/encoder/vp9_block.h"

#ifdef __cplusplus
extern "C" {
#endif

struct encode_b_args {
  MACROBLOCK *x;
  int enable_trellis_opt;
  double trellis_opt_thresh;
  int *sse_calc_done;
  int64_t *sse;
  ENTROPY_CONTEXT *ta;
  ENTROPY_CONTEXT *tl;
  int8_t *skip;
  // Optional output for the token rate of the trellis-optimized block as
  // computed by cost_coeffs() (vp9_rdopt.c) with precise, i.e. non-fast,
  // coefficient costing, or -1 when the rate is unavailable. May be NULL if the
  // caller does not need the rate. See vp9_optimize_b().
  int *rate;
#if CONFIG_MISMATCH_DEBUG
  int mi_row;
  int mi_col;
  int output_enabled;
#endif
};
// Optimizes the quantized coefficients of `block` using trellis RDO and returns
// the final EOB position. If `rate` is non-NULL, `*rate` receives the token
// rate of the optimized block, equal to cost_coeffs() (vp9_rdopt.c) with
// precise, i.e. non-fast, coefficient costing, or -1 when the rate is
// unavailable because the post-EOB sharpness adjustment moved the final EOB.
int vp9_optimize_b(MACROBLOCK *mb, int plane, int block, TX_SIZE tx_size,
                   int ctx, int *rate);
void vp9_encode_sb(MACROBLOCK *x, BLOCK_SIZE bsize, int mi_row, int mi_col,
                   int output_enabled);
void vp9_encode_sby_pass1(MACROBLOCK *x, BLOCK_SIZE bsize);
void vp9_xform_quant_fp(MACROBLOCK *x, int plane, int block, int row, int col,
                        BLOCK_SIZE plane_bsize, TX_SIZE tx_size);
void vp9_xform_quant_dc(MACROBLOCK *x, int plane, int block, int row, int col,
                        BLOCK_SIZE plane_bsize, TX_SIZE tx_size);
void vp9_xform_quant(MACROBLOCK *x, int plane, int block, int row, int col,
                     BLOCK_SIZE plane_bsize, TX_SIZE tx_size);

void vp9_subtract_plane(MACROBLOCK *x, BLOCK_SIZE bsize, int plane);

void vp9_encode_block_intra(int plane, int block, int row, int col,
                            BLOCK_SIZE plane_bsize, TX_SIZE tx_size, void *arg);

// The two steps of vp9_encode_block_intra(): vp9_encode_block_intra_pred()
// writes the prediction of the transform block to the destination buffer and
// returns its transform type, and vp9_encode_block_intra_residual() codes its
// residual and adds it to the prediction.
TX_TYPE vp9_encode_block_intra_pred(int plane, int block, int row, int col,
                                    BLOCK_SIZE plane_bsize, TX_SIZE tx_size,
                                    struct encode_b_args *args);
void vp9_encode_block_intra_residual(int plane, int block, int row, int col,
                                     BLOCK_SIZE plane_bsize, TX_SIZE tx_size,
                                     struct encode_b_args *args);

void vp9_encode_intra_block_plane(MACROBLOCK *x, BLOCK_SIZE bsize, int plane,
                                  int enable_trellis_opt);

#ifdef __cplusplus
}  // extern "C"
#endif

#endif  // VPX_VP9_ENCODER_VP9_ENCODEMB_H_
