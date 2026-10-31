#include "matrix.h"
#include "genmakefile.h"
#include "project.h"
#include "../../lib/fmt.h"
#include "../../lib/scan.h"
#include "../../lib/str.h"
#include "../../lib/strarray.h"

const char *matrix_xtal, *matrix_baud, *matrix_offset;

int
matrix_active(void) {
  return cfg.chip.len && (matrix_xtal || matrix_baud || matrix_offset);
}

/* an offset of zero is no offset */
static int
has_offset(void) {
  unsigned long v = 0;

  if(!matrix_offset)
    return 0;

  scan_xlong(str_start(matrix_offset, "0x") ? matrix_offset + 2 : matrix_offset, &v);
  return v != 0;
}

/* 48 MHz is the internal oscillator with its PLL, no crystal */
static int
is_intosc(void) {
  return matrix_xtal && str_equal(matrix_xtal, "48000000");
}

void
matrix_build_id(stralloc* out) {
  char num[FMT_ULONG];
  unsigned long v;

  stralloc_copys(out, build_types[cfg.build_type]);
  stralloc_lower(out);

  if(matrix_xtal) {
    stralloc_catc(out, '_');

    if(is_intosc()) {
      stralloc_cats(out, "intosc48");
    } else {
      scan_ulong(matrix_xtal, &v);
      num[fmt_ulong(num, v / 1000000)] = '\0';
      stralloc_cats(out, num);
    }

    stralloc_cats(out, "mhz");
  }

  if(matrix_baud) {
    scan_ulong(matrix_baud, &v);
    num[fmt_ulong(num, v / 1000)] = '\0';
    stralloc_catm(out, "_", num, "kbps");
  }

  if(has_offset()) {
    stralloc_cats(out, "_at");
    stralloc_cats(out, str_start(matrix_offset, "0x") ? matrix_offset : "0x");
    stralloc_cats(out, str_start(matrix_offset, "0x") ? "" : matrix_offset);
  }

  stralloc_nul(out);
}

void
matrix_dirs(stralloc* bin, stralloc* obj) {
  stralloc id, target;

  stralloc_init(&id);
  stralloc_init(&target);
  matrix_build_id(&id);

  stralloc_copys(&target, tools.compiler);
  stralloc_catc(&target, '-');
  stralloc_cat(&target, &cfg.chip);
  stralloc_lower(&target);
  stralloc_nul(&target);

  stralloc_catm(bin, "bin/", target.s);
  stralloc_catm(obj, "obj/", target.s, "/", id.s);
  stralloc_nul(bin);
  stralloc_nul(obj);

  stralloc_free(&target);
  stralloc_free(&id);
}

/* NAME=VALUE for the compiler (in DEFS) and for the preprocessor scan */
static void
define(const char* name, const char* value) {
  stralloc sa;

  stralloc_init(&sa);
  stralloc_catm(&sa, name, "=", value);
  stralloc_nul(&sa);
  push_define(sa.s);
  strarray_push(&project_defs, sa.s);
  stralloc_free(&sa);
}

void
matrix_defines(void) {
  stralloc chip, up, name;

  if(!matrix_active())
    return;

  stralloc_init(&chip);
  stralloc_init(&up);
  stralloc_init(&name);
  stralloc_copy(&chip, &cfg.chip);
  stralloc_lower(&chip);
  stralloc_nul(&chip);
  stralloc_copy(&up, &chip);
  stralloc_upper(&up);
  stralloc_nul(&up);

  stralloc_catm(&name, "PIC", up.s);
  stralloc_nul(&name);
  define(name.s, "1");

  /* the compiler setup gives -D__<chip>=1 itself, the preprocessor scan does not know it */
  stralloc_zero(&name);
  stralloc_catm(&name, "__", chip.s);
  stralloc_nul(&name);
  {
    stralloc d;

    stralloc_init(&d);
    stralloc_catm(&d, name.s, "=1");
    stralloc_nul(&d);
    strarray_push(&project_defs, d.s);
    stralloc_free(&d);
  }

  if(matrix_xtal) {
    define("_XTAL_FREQ", matrix_xtal);

    if(is_intosc())
      define("XTAL_USED", "NO_XTAL");
  }

  if(matrix_baud)
    define("UART_BAUD", matrix_baud);

  define(cfg.build_type == BUILD_TYPE_DEBUG ? "__DEBUG" : "__NDEBUG", "1");

  stralloc_free(&name);
  stralloc_free(&up);
  stralloc_free(&chip);
}
