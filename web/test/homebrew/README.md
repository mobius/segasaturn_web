# Ymir Web homebrew disc (original)

Tiny **original** Sega Saturn homebrew image for proving that Ymir Web can load a
disc via `loadDisc` and produce **non-black** framebuffer pixels once a real
512 KiB IPL/BIOS is supplied.

> Do **not** commit or redistribute any BIOS. Place yours locally and pass the path.

## Artifacts

| File | Role |
|------|------|
| `make_homebrew_iso.py` | Generator (re-run anytime) |
| `ymir_homebrew_color.iso` | Mode-1/2048 ISO (also `.bin`+`.cue`) |
| `ip.bin` | 32 KiB system area (System ID + original stubs + AIP) |
| `0.bin` | Hand-assembled SH-2 payload (VDP2 back-color) |

Regenerate:

```bash
python3 web/test/homebrew/make_homebrew_iso.py
```

## What this is / is not

- **Is:** a 40 KiB original ISO with a valid `SEGA SEGASATURN` System ID so Ymir’s
  ISO loader accepts it; includes a tiny SH-2 AIP that would paint a magenta
  back screen if the BIOS transferred control to it.
- **Is not:** a commercial dump; does **not** embed Sega’s copyrighted
  `SYS_SEC.OBJ` / `SYS_ARE*.OBJ`. A retail BIOS will typically fail the security
  check before AIP runs. The **proof still passes** because the real BIOS logo /
  boot path already fills the framebuffer with colorful non-zero pixels.

No SH-2 GCC toolchain was required — the AIP is hand-assembled machine code
inside the generator.

## Proof harness

```bash
# From repo root (wasm already built under build-web/)
YMIR_BIOS=/path/to/saturn_ipl.bin node web/test/homebrew_proof.cjs

# Or argv:
node web/test/homebrew_proof.cjs /path/to/saturn_ipl.bin
```

Success criteria (defaults):

- `loadIPL` + `loadDisc(homebrew iso)` succeed
- After ~180 frames: ≥1000 nonzero pixels and ≥500 “colorful” pixels
  (`max(R,G,B) >= 40`)
- Writes `build-web/homebrew-proof.png`

Optional env: `YMIR_FRAMES`, `YMIR_DISC`, `YMIR_OUT`, `YMIR_MIN_NONZERO`,
`YMIR_MIN_COLORFUL`.

## Note on wasm stack

Real BIOS execution needs a larger Emscripten stack than the default 64 KiB.
`web/CMakeLists.txt` sets `-sSTACK_SIZE=8388608`. Rebuild if you change it.

## Git / redistribution

Repo `.gitignore` ignores `*.iso` / `*.cue` (and all BIOS/ROMs). Keep the generator
committed if you wish; re-run `make_homebrew_iso.py` to recreate the disc locally.
Never commit files from `/workspace/saturn-bios-drop/` or any IPL dump.
