# Google Patch Rewards — libvpx local draft notes

**Date:** 2026-09-09 (America/Chicago)  
**Do not claim yet:** need upstream merge + ≥30 days, then https://bughunters.google.com/report/patch_rewards

## Chosen target + why

- **Project:** libvpx (WebM / Chromium)
- **Upstream:** https://chromium.googlesource.com/webm/libvpx (Gerrit; same flow as libwebp)
- **Local clone:** `/workspace/google-patch-libvpx`
- **Branch:** `local/tilebuf-fbounds-safety` (from `main` @ `d2413e2ca`)
- **Why this target:**
  1. Tier-1 **Core infrastructure data parsers** (3× memory-safety multiplier through end-2026).
  2. Clear, mergeable first-CL scope: **one** struct field pair (`TileBuffer.data` / `size`) on the VP9 tile parse/decode path for untrusted compressed tile bytes — not a whole-library sweep.
  3. Same pattern as libwebp demux / libpng `read_buffer`: **inert macros** when the flag is off; experimental Clang `-fbounds-safety` only when explicitly enabled.
  4. `TileBuffer` is decoder-private (safe to declare size before pointer for `counted_by` scope); no public ABI churn.
  5. Not already done upstream (no `__counted_by` / `-fbounds-safety` in tree).

## Security benefit

`TileBuffer` holds a pointer+length view into untrusted VP9 bitstream tile payloads before `setup_token_decoder` / bool decoding. Callers already track capacity in `size`, but the compiler cannot see that relationship.

This draft:

1. Introduces `vpx_ports/bounds_safety.h` with `VPX_COUNTED_BY` / `VPX_COUNTED_BY_OR_NULL` (empty by default).
2. Annotates `TileBuffer.data` with `VPX_COUNTED_BY_OR_NULL(size)` (nullable-friendly).
3. Declares `size` **before** `data` so the named count is in scope for the attribute.
4. Makes `get_tile_buffer` **capacity-then-pointer** so counted-by invariants hold under a real `-fbounds-safety` build.
5. Wires optional configure `--enable-fbounds-safety` (OFF by default) → `-DVPX_SUPPORT_FBOUNDS_SAFETY` + `-fbounds-safety`.

**Default builds are unchanged:** macros expand to nothing; no new runtime checks without the experimental flag.

## Files changed

| File | Change |
|------|--------|
| `vpx_ports/bounds_safety.h` | **New** — inert / Clang bounds macros |
| `vpx_ports/vpx_ports.mk` | List new header in PORTS_SRCS |
| `vp9/decoder/vp9_decoder.h` | Include macros; annotate `TileBuffer`; size before data |
| `vp9/decoder/vp9_decodeframe.c` | Capacity-first assign in `get_tile_buffer` |
| `configure` | `--enable-fbounds-safety` option (default off) |
| `NOTES.md` | This file |

## How to build / test

Default (macros inert — must stay green):

```sh
./configure --disable-examples --disable-tools --disable-docs --disable-unit-tests
make -j$(nproc)
```

With experimental bounds-safety toolchain (maintainers / CI; **not** available on typical boxes without Clang/`ptrcheck.h`):

```sh
CC=<clang-with-fbounds-safety> ./configure --enable-fbounds-safety \
  --disable-examples --disable-tools --disable-docs
make -j$(nproc)
```

## Upstream / Gerrit plan

1. Sign Google CLA if needed: https://cla.developers.google.com/
2. Install Gerrit commit-msg hook:
   `curl -Lo .git/hooks/commit-msg https://chromium-review.googlesource.com/tools/hooks/commit-msg && chmod u+x .git/hooks/commit-msg`
3. Proposed commit subject:
   `vp9: add optional -fbounds-safety annotation for TileBuffer`
4. Cite libwebp demux / utils `-fbounds-safety` prior art; frame as secure-by-design / Safe Buffers-style systematization.
5. Upload when parent is ready (not done in this draft):
   `git push https://chromium-review.googlesource.com/webm/libvpx HEAD:refs/for/main`
6. Do **not** open a GitHub PR against a read-only mirror.

## Follow-ups (separate CLs)

- `vpx_codec_frame_buffer_t.data` / `size` (public decode frame buffers; mind ABI / assignment sites in app callbacks)
- `vpx_reader.buffer` / `buffer_end` (bitreader — end-pointer style, not size field)
- IVF `video_reader` / `ivf_read_frame` buffer+`buffer_size` (tools demux path)

## Claim reminder

- Submit to Google claim form only **after merge** and waiting **≥ 30 days**.
- Claim URL: https://bughunters.google.com/report/patch_rewards
- Category framing: secure-by-design memory safety / Safe Buffers-style bounds systematization on a Tier-1 VP9 tile parser path.

## Status

**LOCAL DRAFT** — branch `local/tilebuf-fbounds-safety`; no Gerrit upload; no claim.
