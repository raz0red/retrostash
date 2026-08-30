/* WRC - minimal replacement for the original flycast_stubs.o (bitcode,
 * source lost). That file also defined its own
 * fill_short_pathname_representation[_noext] and bm_Reset, duplicating
 * real implementations that already exist in flycast's own core
 * (core/deps/libretro-common/file/file_path.c,
 * core/hw/sh4/dyna/blockmanager.cpp) with an incompatible signature -
 * confirmed by wasm-ld emitting a "signature_mismatch:" trap stub for
 * fill_short_pathname_representation at the exact crash site. The only
 * symbol actually needed from the original stub file is this one, for
 * -Wl,--wrap=glGetString. Passing straight through to the real function.
 *
 * Rebuild with: emcc -c flycast_glwrap_stub.c -O2 -o flycast_glwrap_stub.o
 */

typedef unsigned int GLenum;
typedef unsigned char GLubyte;

extern const GLubyte *__real_glGetString(GLenum name);

const GLubyte *__wrap_glGetString(GLenum name)
{
   return __real_glGetString(name);
}
