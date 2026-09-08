# Building this core for webrcade (WASM)

Unlike `mupen64plus-libretro-nx`, this core ships as a **single WASM
variant** (`-s FULL_ES2=1` is unconditional in
`RetroArch/Makefile.emscripten.parallel_n64` - there's no WebGL2/legacy
toggle here) and links via the older **archive-based** approach
(`libretro_emscripten.bc` copied to `libretro_emscripten.a`), not the
direct-object-linking (`CORE_OBJS`) approach `mupen64plus-libretro-nx`
uses. That means, unlike that core, copying the built `.bc` file into
`RetroArch/` genuinely is a required step here, not a vestigial no-op.

**Note:** this doc is written from reading the Makefiles directly, not
from a build verified step-by-step in the same session as
`mupen64plus-libretro-nx`'s (see that core's own `BUILDING-WRC.md` for the
much more hard-won set of gotchas around that one - things like "always
`make clean` before every build" and "never trust an exit code piped
through `tail`" are good practice here too, just not independently
re-confirmed for this specific core).

## Prerequisites

- Docker container `emsdk-3.1.74` running (emscripten SDK 3.1.74).
- This repo (`parallel-n64-ejs`) and `RetroArch` both live under
  `retrostash/` and are bind-mounted into the container at
  `/work/webrcade/retrostash/...`.
- On Windows/Git Bash, prefix any `docker exec` command whose arguments
  contain `/work/...`-style paths with `MSYS_NO_PATHCONV=1`, or Git Bash
  will mangle them into Windows paths.

## Build

Core build (from `retrostash/parallel-n64-ejs`) - produces
`parallel_n64_libretro_emscripten.bc`:
```
MSYS_NO_PATHCONV=1 docker exec emsdk-3.1.74 sh -c "cd /work/webrcade/retrostash/parallel-n64-ejs && emmake make platform=emscripten clean && emmake make platform=emscripten -j7 > /tmp/build_parallel_n64.log 2>&1; echo REAL_EXIT_CODE:\$?"
```

Copy the archive into RetroArch's directory (required for this core,
unlike mupen64plus-next - see note above):
```
cp parallel-n64-ejs/parallel_n64_libretro_emscripten.bc RetroArch/libretro_emscripten.bc
```

RetroArch relink (from `retrostash/RetroArch`):
```
MSYS_NO_PATHCONV=1 docker exec emsdk-3.1.74 sh -c "cd /work/webrcade/retrostash/RetroArch && rm -rf obj-emscripten2 libretro_emscripten.a parallel_n64_libretro.js parallel_n64_libretro.wasm parallel_n64_libretro.wasm.map && emmake make -f Makefile.emscripten.parallel_n64 LIBRETRO=parallel_n64 -j7 > /tmp/link_parallel_n64.log 2>&1; echo REAL_EXIT_CODE:\$?"
```

Deploy (paths relative to `retrostash/`, into the app's `public/js/`):
```
cp RetroArch/parallel_n64_libretro.js  ../webrcade-app-retro-parallel-n64/public/js/parallel_n64_libretro.js
cp RetroArch/parallel_n64_libretro.wasm ../webrcade-app-retro-parallel-n64/public/js/parallel_n64_libretro.wasm
```

## If something doesn't match what's deployed

Before assuming a rebuild fixed (or didn't fix) something, verify the
*actual deployed file* directly rather than trusting a browser reload -
both stale caching and (for `mupen64plus-libretro-nx`) build-process
quirks have both produced misleading "the fix didn't work" symptoms this
project. From inside the container:
```
node -e "
const fs = require('fs');
const bytes = fs.readFileSync('/path/to/the.wasm');
const mod = new WebAssembly.Module(bytes);
console.log(WebAssembly.Module.imports(mod).length);
"
```
