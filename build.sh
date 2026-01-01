#!/bin/bash

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )

#cd $SCRIPT_DIR/SameBoy/libretro
#emmake make -f Makefile -j6 platform=emscripten
#cp ../*.bc $SCRIPT_DIR/RetroArch/dist-scripts-new/

#cd $SCRIPT_DIR/Genesis-Plus-GX
#emmake make -f Makefile.libretro -j6 platform=emscripten
#cp *.bc $SCRIPT_DIR/RetroArch/dist-scripts-new/

#cd $SCRIPT_DIR/mgba
#emmake make -f Makefile -j6 platform=emscripten
#cp *.bc $SCRIPT_DIR/RetroArch/dist-scripts-new/

# cd $SCRIPT_DIR/beetle-psx-libretro
# emmake make -f Makefile -j6 platform=emscripten
# cp *.bc $SCRIPT_DIR/RetroArch/dist-scripts-new/

#cd $SCRIPT_DIR/beetle-lynx-libretro
#emmake make -f Makefile -j6 platform=emscripten
#cp *.bc $SCRIPT_DIR/RetroArch/dist-scripts-new/

#cd $SCRIPT_DIR/beetle-ngp-libretro-new
#emmake make -f Makefile -j6 platform=emscripten
#cp *.bc $SCRIPT_DIR/RetroArch/dist-scripts-new/

#cd $SCRIPT_DIR/beetle-supergrafx-libretro
#emmake make -f Makefile -j6 platform=emscripten
#cp *.bc $SCRIPT_DIR/RetroArch/dist-scripts-new/

#cd $SCRIPT_DIR/beetle-vb-libretro
#emmake make -f Makefile -j6 platform=emscripten
#cp *.bc $SCRIPT_DIR/RetroArch/dist-scripts-new/

#cd $SCRIPT_DIR/beetle-wswan-libretro
#emmake make -f Makefile -j6 platform=emscripten
#cp *.bc $SCRIPT_DIR/RetroArch/dist-scripts-new/

#cd $SCRIPT_DIR/Genesis-Plus-GX
#emmake make -f Makefile.libretro -j6 platform=emscripten
#cp *.bc $SCRIPT_DIR/RetroArch/dist-scripts-new/

#cd $SCRIPT_DIR/beetle-pce-fast-libretro
#emmake make -f Makefile -j6 platform=emscripten
#cp *.bc $SCRIPT_DIR/RetroArch/dist-scripts-new/

# cd $SCRIPT_DIR/beetle-saturn-libretro
# emmake make -f Makefile -j6 platform=emscripten
# cp *.bc $SCRIPT_DIR/RetroArch/dist-scripts-new/

#cd $SCRIPT_DIR/beetle-pcfx-libretro
#emmake make -f Makefile -j6 platform=emscripten
#cp *.bc $SCRIPT_DIR/RetroArch/dist-scripts-new/

#cd $SCRIPT_DIR/neocd_libretro
#emmake make -f Makefile -j6 platform=emscripten
#cp *.bc $SCRIPT_DIR/RetroArch/dist-scripts-new/

#cd $SCRIPT_DIR/a5200
#emmake make -f Makefile -j6 platform=emscripten
#cp *.bc $SCRIPT_DIR/RetroArch/dist-scripts-new/

#cd $SCRIPT_DIR/ecwolf/src/libretro
#emmake make -f Makefile -j6 platform=emscripten
#cp *.bc $SCRIPT_DIR/RetroArch/dist-scripts-new/

#cd $SCRIPT_DIR/tyrquake-new
#emmake make -f Makefile -j6 platform=emscripten
#cp *.bc $SCRIPT_DIR/RetroArch/dist-scripts-new/

#cd $SCRIPT_DIR/parallel-n64
#emmake make -f Makefile -j6 platform=emscripten
#cp *.bc $SCRIPT_DIR/RetroArch/dist-scripts-new/

cd $SCRIPT_DIR/same_cdi
emmake make -f Makefile.libretro -j6 platform=emscripten
cp *.bc $SCRIPT_DIR/RetroArch/dist-scripts-new/

#cd $SCRIPT_DIR/opera-libretro
#emmake make -f Makefile -j6 platform=emscripten
#cp *.bc $SCRIPT_DIR/RetroArch/dist-scripts-new/

# REQUIRES THREADING IN MAKEFILE
#cd $SCRIPT_DIR/yabause.old/yabause/src/libretro
#cd $SCRIPT_DIR/threaded/yabause/yabause/src/libretro
#emmake make -f Makefile -j6 platform=emscripten
#cp *.bc $SCRIPT_DIR/RetroArch/dist-scripts-new/

# cd $SCRIPT_DIR/daphne
# emmake make -f Makefile -j6 platform=emscripten
# cp *.bc $SCRIPT_DIR/RetroArch/dist-scripts-new/

#cd $SCRIPT_DIR/stella2014-libretro
#emmake make -f Makefile -j6 platform=emscripten
#cp *.bc $SCRIPT_DIR/RetroArch/dist-scripts-new/

#cd $SCRIPT_DIR/prosystem-libretro-update
#emmake make -f Makefile -j6 platform=emscripten
#cp *.bc $SCRIPT_DIR/RetroArch/dist-scripts-new/

#cd $SCRIPT_DIR/stella/src/os/libretro
#emmake make -f Makefile -j6 platform=emscripten
#cp *.bc $SCRIPT_DIR/RetroArch/dist-scripts-new/

#cd $SCRIPT_DIR/vice-libretro
#emmake make -f Makefile -j6 platform=emscripten
#cp *.bc $SCRIPT_DIR/RetroArch/dist-scripts-new/

#cd $SCRIPT_DIR/libretro-fceumm
#emmake make -f Makefile -j6 platform=emscripten
#cp *.bc $SCRIPT_DIR/RetroArch/dist-scripts-new/

# REQUIRES THREADING IN MAKEFILE
#cd $SCRIPT_DIR/dosbox-pure
#emmake make -f Makefile -j6 platform=emscripten
#cp *.bc $SCRIPT_DIR/RetroArch/dist-scripts-new/

# REQUIRES THREADING IN MAKEFILE
#cd $SCRIPT_DIR/melonDS
#emmake make -f Makefile -j6 platform=emscripten
#cp *.bc $SCRIPT_DIR/RetroArch/dist-scripts-new/

#cd $SCRIPT_DIR/dosbox-libretro
#emmake make -f Makefile -j6 platform=emscripten
#cp *.bc $SCRIPT_DIR/RetroArch/dist-scripts-new/

#cd $SCRIPT_DIR/PokeMini
#emmake make -f Makefile -j6 platform=emscripten
#cp *.bc $SCRIPT_DIR/RetroArch/dist-scripts-new/

#cd $SCRIPT_DIR/snes9x/libretro
#emmake make -f Makefile -j6 platform=emscripten
#cp *.bc $SCRIPT_DIR/RetroArch/dist-scripts-new/

cd $SCRIPT_DIR/RetroArch/dist-scripts-new/
emmake ./dist-cores.sh emscripten

cd $SCRIPT_DIR
