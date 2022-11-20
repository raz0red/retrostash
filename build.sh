#!/bin/bash

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )

#cd $SCRIPT_DIR/beetle-psx-libretro
#emmake make -f Makefile -j6 platform=emscripten
#cp *.bc $SCRIPT_DIR/RetroArch/dist-scripts/

#cd $SCRIPT_DIR/Genesis-Plus-GX
#emmake make -f Makefile.libretro -j6 platform=emscripten
#cp *.bc $SCRIPT_DIR/RetroArch/dist-scripts/

cd $SCRIPT_DIR/beetle-pce-fast-libretro
emmake make -f Makefile -j6 platform=emscripten
cp *.bc $SCRIPT_DIR/RetroArch/dist-scripts/


cd $SCRIPT_DIR/RetroArch/dist-scripts/
emmake ./dist-cores.sh emscripten

cd $SCRIPT_DIR