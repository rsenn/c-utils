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

/* --universal-hex (1), --no-universal-hex (0), -1: on for PIC18 with a code offset */
extern int xc8_universal_opt;

/* the compiler is the modern XC8 driver (xc8-cc) */
int xc8_is_cc(void);

/* put --cflag, --ldflag and the code offset into CFLAGS and LDFLAGS */
void xc8_apply(void);

/* the --ldflag options, after the directories are known */
void xc8_apply_late(void);

/* ninja: the universal hex rule and its text */
struct target_s;
int xc8_is_vectors_rule(const struct target_s* rule);
void xc8_ninja_rules(buffer* out);

/* XC8_BIN, PATH and DFP_DIR defaults at the top of the makefile */
void xc8_preamble(buffer* out);

/* a rule for the .hex next to each .elf */
void xc8_hex_rules(void);

/* patterns of the files the xc8-cc driver leaves besides the image */
void xc8_clean_args(strlist* args, char psm);

#endif /* defined(GENMAKEFILE_XC8_H) */
