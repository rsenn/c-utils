#ifndef GENMAKEFILE_MATRIX_H
#define GENMAKEFILE_MATRIX_H

#include "../../lib/stralloc.h"

/* The build of a microcontroller firmware is one point of a matrix: the
   compiler, the chip, the build type, the clock, the baud rate of the serial
   port and the address the program is loaded to. Each point gets a
   file of its own, which writes its objects below
   obj/<compiler>-<chip>/<build id>/<program>/ and its binaries to
   bin/<compiler>-<chip>/<program>_<build id>.<ext>, where the build id is
   <type>_<mhz>mhz_<kbps>kbps[_at<offset>]. */

/* --xtal HZ, --baud BPS, --code-offset ADDRESS */
extern const char *matrix_xtal, *matrix_baud, *matrix_offset;

/* the layout above is used when one of them was given */
int matrix_active(void);

/* build id, as in debug_20mhz_38kbps */
void matrix_build_id(stralloc* out);

/* directories of the binaries and of the objects, below the output directory */
void matrix_dirs(stralloc* bin, stralloc* obj);

/* macros of the matrix point: given to the compiler as -D options, and to the
   preprocessor scan */
void matrix_defines(void);

#endif /* defined(GENMAKEFILE_MATRIX_H) */
