# Building this core for webrcade (WASM)

This core ships as **two separate WASM variants**: the default WebGL2/GLES3
build, and a GLES2/WebGL1 legacy build (used for titles that don't render
correctly under WebGL2, e.g. 1080 Snowboarding). They are built and linked
separately and **cannot share intermediate object files** - the Makefiles
don't tag `.o` files by variant, so building one after the other reuses
(and silently corrupts) the previous variant's objects unless you rebuild
from a clean state every time.

## Prerequisites

- Docker container `emsdk-3.1.74` running (emscripten SDK 3.1.74).
- This repo (`mupen64plus-libretro-nx`) and `RetroArch` both live under
  `retrostash/` and are bind-mounted into the container at
  `/work/webrcade/retrostash/...`.
- On Windows/Git Bash, prefix any `docker exec` command whose arguments
  contain `/work/...`-style paths with `MSYS_NO_PATHCONV=1`, or Git Bash
  will mangle them into Windows paths.

## Critical rules (learned the hard way)

1. **Always run `make clean` before building either variant, every time**,
   even if you just built the *same* variant a moment ago. Relinking
   RetroArch against this core's objects appears to consume/invalidate a
   subset of the `.o` files as a side effect (most likely related to
   `LTO=1` / `--llvm-lto 3` bitcode processing) - a core build that
   succeeded five minutes ago is not a safe thing to relink against a
   second time. Treat "rebuild core -> relink RetroArch -> deploy" as one
   atomic sequence per variant, never as separable steps you can redo out
   of order.
2. **Never trust an exit code shown after piping through `tail`.** A
   command like `make ... | tail -60` reports `tail`'s exit code, not
   `make`'s - a real build failure can look like `exited with code 0`.
   Always redirect to a log file and check the exit code separately:
   ```
   emmake make ... > /tmp/build.log 2>&1; echo REAL_EXIT_CODE:$?
   ```
   then `tail`/`grep` the log file afterward if you want a summary.
3. **The GLES2 variant must be built with the correct output filename from
   the start** - pass `TARGET=mupen64plus_next_libretro_gles2.js` on the
   RetroArch link command, don't build it as the default
   `mupen64plus_next_libretro.js` and rename the files afterward.
   Emscripten's generated JS glue can end up referencing the `.wasm`
   filename it was built with; renaming the deployed files post-hoc does
   not update that reference, and the mismatch produces a WASM
   `LinkError: ... function import requires a callable` that looks like a
   core code bug but isn't - it's the JS loading the wrong (WebGL2-built)
   `.wasm` binary under the GLES2 build's name.
4. **Copying the `.bc` archive between the core directory and RetroArch is
   not necessary and does nothing** for this build path. RetroArch's
   `Makefile.emscripten.mupen64plus-next` links directly against this
   core's individual `.o` files via a `CORE_OBJS` list of relative paths
   (`../mupen64plus-libretro-nx/GLideN64/src/...o`, etc.), not against
   `libretro_emscripten.bc`. What actually determines which variant gets
   linked is simply whichever `.o` files are currently sitting on disk in
   this core's build tree - hence rule #1.
5. **When debugging a WASM import/link mismatch, don't trust
   `wasm-dis`/`llvm-nm` text search on the compiled objects to be
   conclusive** - the ground truth is what the browser's own
   `WebAssembly.Module.imports()` reports for the *actual deployed file*.
   From inside the container:
   ```
   node -e "
   const fs = require('fs');
   const bytes = fs.readFileSync('/path/to/the.wasm');
   const mod = new WebAssembly.Module(bytes);
   console.log(WebAssembly.Module.imports(mod).length);
   console.log(WebAssembly.Module.imports(mod)[135]); // match the browser's Import #N
   "
   ```
   This also catches stale browser/service-worker caching: if the deployed
   file's import list doesn't match what the browser reports, the browser
   is not loading the file you think it is.

## Variant 1: Default (WebGL2 / GLES3)

Core build (from `retrostash/mupen64plus-libretro-nx`):
```
MSYS_NO_PATHCONV=1 docker exec emsdk-3.1.74 sh -c "cd /work/webrcade/retrostash/mupen64plus-libretro-nx && emmake make platform=emscripten clean && emmake make platform=emscripten -j7 > /tmp/build.log 2>&1; echo REAL_EXIT_CODE:\$?"
```

RetroArch relink (from `retrostash/RetroArch`) - run immediately after, same
session:
```
MSYS_NO_PATHCONV=1 docker exec emsdk-3.1.74 sh -c "cd /work/webrcade/retrostash/RetroArch && rm -rf obj-emscripten2 mupen64plus_next_libretro.js mupen64plus_next_libretro.wasm mupen64plus_next_libretro.wasm.map && emmake make -f Makefile.emscripten.mupen64plus-next LIBRETRO=mupen64plus_next WEBGL2=1 -j7 > /tmp/link.log 2>&1; echo REAL_EXIT_CODE:\$?"
```

Deploy (paths relative to `retrostash/`, into the app's `public/js/`):
```
cp RetroArch/mupen64plus_next_libretro.js  ../webrcade-app-retro-mupen64plus-next/public/js/mupen64plus_next_libretro.js
cp RetroArch/mupen64plus_next_libretro.wasm ../webrcade-app-retro-mupen64plus-next/public/js/mupen64plus_next_libretro.wasm
```

## Variant 2: Legacy (GLES2 / WebGL1)

Used when the app forces `disableOpenGL2` (see `NO_WEBGL2_MD5S` in
`webrcade-app-retro-mupen64plus-next/src/emulator/index.js`).

Core build - note `EMULATORJS_LEGACY=1`, which flips `GLES3` to `0` in the
Makefile:
```
MSYS_NO_PATHCONV=1 docker exec emsdk-3.1.74 sh -c "cd /work/webrcade/retrostash/mupen64plus-libretro-nx && emmake make platform=emscripten clean && emmake make platform=emscripten EMULATORJS_LEGACY=1 -j7 > /tmp/build_gles2.log 2>&1; echo REAL_EXIT_CODE:\$?"
```

RetroArch relink - note `TARGET=` is set explicitly (see rule #3 above),
and `WEBGL2=0`:
```
MSYS_NO_PATHCONV=1 docker exec emsdk-3.1.74 sh -c "cd /work/webrcade/retrostash/RetroArch && rm -rf obj-emscripten2 mupen64plus_next_libretro_gles2.js mupen64plus_next_libretro_gles2.wasm mupen64plus_next_libretro_gles2.wasm.map && emmake make -f Makefile.emscripten.mupen64plus-next LIBRETRO=mupen64plus_next TARGET=mupen64plus_next_libretro_gles2.js WEBGL2=0 -j7 > /tmp/link_gles2.log 2>&1; echo REAL_EXIT_CODE:\$?"
```

Deploy:
```
cp RetroArch/mupen64plus_next_libretro_gles2.js  ../webrcade-app-retro-mupen64plus-next/public/js/mupen64plus_next_libretro_gles2.js
cp RetroArch/mupen64plus_next_libretro_gles2.wasm ../webrcade-app-retro-mupen64plus-next/public/js/mupen64plus_next_libretro_gles2.wasm
```

## Debug symbols (DWARF) - not used in normal builds

The default/shipped build strips debug info (no `-g`, no
`-gsource-map`, default `--gc-sections`). If you ever need real
symbolicated crash traces again, see the `project_n64_debug_symbols_root_cause`
memory for the full story - short version: `wasm-ld`'s `-s` alias for
`--strip-all` was historically getting invoked by accident via an orphaned
flag in `RetroArch/Makefile.emscripten.mupen64plus-next`'s `LDFLAGS`, and
getting real DWARF through also requires linking this core's objects
directly (already how `CORE_OBJS` works) rather than via the `.a`/`.bc`
archive, since `wasm-ld` drops debug sections from lazily-extracted
archive members. A debug build adds ~30MB to the `.wasm` and should never
be what's deployed to players.
