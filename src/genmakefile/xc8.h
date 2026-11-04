#ifndef GENMAKEFILE_XC8_H
#define GENMAKEFILE_XC8_H

#include "../../lib/buffer.h"
#include "../../lib/strarray.h"
#include "../../lib/strlist.h"

/* flags of --cflag (compile and link) and --ldflag (link only) */
extern strarray xc8_cflags, xc8_ldflags;

/* --pack VENDOR/NAME/VERSION (device family pack), --toolchain-version V,
   --program NAME (the name of the image) */
extern const char *xc8_pack, *xc8_version, *xc8_program;

/* the compiler is the modern XC8 driver (xc8-cc) */
int xc8_is_cc(void);

/* put --cflag, --ldflag and the code offset into CFLAGS and LDFLAGS */
void xc8_apply(void);

/* XC8_BIN, PATH and DFP_DIR defaults at the top of the makefile */
void xc8_preamble(buffer* out);

/* a rule for the .hex next to each .elf */
void xc8_hex_rules(void);

/* patterns of the files the xc8-cc driver leaves besides the image */
void xc8_clean_args(strlist* args, char psm);

#endif /* defined(GENMAKEFILE_XC8_H) */
