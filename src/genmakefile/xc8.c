#include "xc8.h"
#include "genmakefile.h"
#include "matrix.h"
#include "compiler.h"
#include "../../lib/env.h"
#include "../../lib/arena.h"

strarray xc8_cflags, xc8_ldflags;
const char *xc8_pack, *xc8_version, *xc8_program;

int
xc8_is_cc(void) {
  return tools.compiler && str_start(tools.compiler, "xc8-cc");
}

/* 0x2000, 2000 and 0X2000 as 0x2000; 0 when there is no offset */
static int
code_offset(stralloc* out) {
  unsigned long v = 0;
  const char* s;

  if(!matrix_offset)
    return 0;

  s = str_start(matrix_offset, "0x") || str_start(matrix_offset, "0X") ? matrix_offset + 2 : matrix_offset;
  scan_xlong(s, &v);

  if(!v)
    return 0;

  stralloc_copys(out, "0x");
  stralloc_cats(out, s);
  stralloc_nul(out);
  return 1;
}

void
xc8_apply(void) {
  char** p;
  stralloc off;

  if(!xc8_is_cc())
    return;

  stralloc_init(&off);

  /* the driver wants it when it compiles and when it links */
  if(code_offset(&off)) {
    stralloc opt;

    stralloc_init(&opt);
    stralloc_catm(&opt, "-mcodeoffset=", off.s);
    stralloc_nul(&opt);
    var_push("CFLAGS", opt.s);
    stralloc_free(&opt);
  }

  strarray_foreach(&xc8_cflags, p) var_push("CFLAGS", *p);
  strarray_foreach(&xc8_ldflags, p) var_push("LDFLAGS", *p);

  stralloc_free(&off);
}

/* directory DIR/vVERSION/bin of a root, when it holds xc8-cc */
static int
find_bin(stralloc* out) {
  static const char* roots[] = {"/opt/microchip/xc8", "/mnt/data/opt/microchip/xc8", "/usr/local/microchip/xc8", 0};
  stralloc dir, exe;
  const char* home = env_get("HOME");
  int i, ret = 0;

  if(!xc8_version)
    return 0;

  stralloc_init(&dir);
  stralloc_init(&exe);

  for(i = home ? -1 : 0; roots[i < 0 ? 0 : i] && !ret; ++i) {
    stralloc_zero(&dir);

    if(i < 0)
      stralloc_catm(&dir, home, "/opt/microchip/xc8");
    else
      stralloc_cats(&dir, roots[i]);

    stralloc_catm(&dir, "/v", xc8_version, "/bin");
    stralloc_nul(&dir);
    stralloc_copy(&exe, &dir);
    stralloc_cats(&exe, "/xc8-cc");
    stralloc_nul(&exe);

    if(path_exists(exe.s)) {
      stralloc_copy(out, &dir);
      ret = 1;
    }
  }

  stralloc_free(&dir);
  stralloc_free(&exe);
  return ret;
}

/* PACKS/VENDOR/NAME/VERSION below the pack directories of MPLAB X */
static int
find_dfp(stralloc* out) {
  static const char* bases[] = {"/opt/microchip/mplabx", "/mnt/data/opt/microchip/mplabx", 0};
  stralloc rel, dir;
  const char* home = env_get("HOME");
  int i, ret = 0;

  if(!xc8_pack)
    return 0;

  stralloc_init(&rel);
  stralloc_init(&dir);
  stralloc_catm(&rel, "/", xc8_pack);
  stralloc_nul(&rel);

  if(home) {
    stralloc_catm(&dir, home, "/.mchp_packs", rel.s);
    stralloc_nul(&dir);

    if(path_is_directory(dir.s)) {
      stralloc_copy(out, &dir);
      ret = 1;
    }
  }

  for(i = 0; bases[i]; ++i) {
    arena ar;
    struct dirlist dl;
    char* rec;

    arena_init(&ar, &arena_heap, 0);
    dl = dirlist_read(&ar, bases[i], DIRLIST_SORT);

    if(dl.s)
      /* sorted by name: a later version wins */
      dirlist_foreach(rec, dl) {
        if(dirlist_name(rec)[0] != 'v')
          continue;

        stralloc_zero(&dir);
        stralloc_catm(&dir, bases[i], "/", dirlist_name(rec), "/packs", rel.s);
        stralloc_nul(&dir);

        if(path_is_directory(dir.s)) {
          stralloc_copy(out, &dir);
          ret = 1;
        }
      }

    arena_free(&ar);
  }

  stralloc_free(&rel);
  stralloc_free(&dir);
  return ret;
}

void
xc8_preamble(buffer* out) {
  stralloc bin, dfp;
  int gmake;

  if(!xc8_is_cc() || build_tool == TOOL_NINJA || build_tool == TOOL_BATCH || build_tool == TOOL_SHELL)
    return;

  gmake = str_equal(tools.make, "gmake");

  stralloc_init(&bin);
  stralloc_init(&dfp);

  if(find_bin(&bin)) {
    stralloc_nul(&bin);
    output_default(out, build_tool, "XC8_BIN", bin.s);

    if(gmake)
      buffer_putm_internal(out, "export PATH := $(XC8_BIN):$(PATH)", newline, NULL);
  }

  /* the -mdfp option names the pack, make has no default for it otherwise */
  if(find_dfp(&dfp)) {
    stralloc_nul(&dfp);
    output_default(out, build_tool, "DFP_DIR", dfp.s);
  } else if(gmake) {
    buffer_putm_internal(out,
                         "ifndef DFP_DIR",
                         newline,
                         "$(warning DFP_DIR is not set: it is the directory of the device family pack ",
                         xc8_pack ? xc8_pack : "(Microchip/<pack>/<version>)",
                         ")",
                         newline,
                         "endif",
                         newline,
                         NULL);
  }

  buffer_putnl(out, 0);
  buffer_flush(out);

  stralloc_free(&bin);
  stralloc_free(&dfp);
}

void
xc8_hex_rules(void) {
  MAP_PAIR_T t;
  strlist images;
  const char* x;
  size_t n;

  if(!xc8_is_cc() || !exts.bin || !str_equal(exts.bin, ".elf"))
    return;

  strlist_init(&images, '\0');

  MAP_FOREACH(rule_map, t) {
    target* rule = MAP_ITER_VALUE(t);
    set_iterator_t it;

    if(rule->phony || !set_size(&rule->prereq) || !rule->recipe.s)
      continue;

    set_foreach(&rule->output, it, x, n) {
      if(n > 4 && !byte_diff(x + n - 4, 4, ".elf"))
        strlist_pushb_unique(&images, x, n);
    }
  }

  {
    target* all = rule_get("all");
    char* img;

    strlist_foreach_s(&images, img) {
      stralloc hex;
      target* rule;

      stralloc_init(&hex);
      stralloc_catb(&hex, img, str_len(img) - 4);
      stralloc_cats(&hex, ".hex");
      stralloc_nul(&hex);

      /* the driver writes the .hex when it links the .elf */
      rule = rule_get_sa(&hex);
      set_adds(&rule->prereq, img);
      add_path_b(&all->prereq, hex.s, hex.len);
      stralloc_free(&hex);
    }
  }

  strlist_free(&images);
}

/* files that xc8-cc leaves beside the objects and the image */
static const char* const leftovers[] = {
    ".hex", ".map", ".d", ".i", ".o", ".sdb", ".sym", ".cmf", ".hxl", ".mum", ".as", ".lst", ".rlf", ".elf", ".s", 0};

void
xc8_clean_args(strlist* args, char psm) {
  MAP_PAIR_T t;
  stralloc fn;

  if(!xc8_is_cc())
    return;

  stralloc_init(&fn);

  MAP_FOREACH(rule_map, t) {
    target* rule = MAP_ITER_VALUE(t);
    set_iterator_t it;
    const char* x;
    size_t n;

    if(rule->phony)
      continue;

    set_foreach(&rule->output, it, x, n) {
      size_t i, dot, slash;

      if(byte_chr(x, n, '%') < n)
        continue;

      /* the objects and images: the directory is shared, so the
         patterns cover it; a name in the current directory only by name */
      if(!((n > 3 && !byte_diff(x + n - 3, 3, ".p1")) || (n > 4 && !byte_diff(x + n - 4, 4, ".elf"))))
        continue;

      slash = byte_rchr(x, n, psm);
      dot = byte_rchr(x, n, '.');

      for(i = 0; leftovers[i]; ++i) {
        stralloc_zero(&fn);

        if(slash < n) {
          stralloc_catb(&fn, x, slash + 1);
          stralloc_catc(&fn, '*');
        } else {
          stralloc_catb(&fn, x, dot < n ? dot : n);
        }

        stralloc_cats(&fn, leftovers[i]);
        stralloc_nul(&fn);
        strlist_push_unique(args, fn.s);
      }

      /* the link also writes the memory summary and the startup code */
      if(n > 4 && !byte_diff(x + n - 4, 4, ".elf")) {
        stralloc_zero(&fn);
        if(slash < n)
          stralloc_catb(&fn, x, slash + 1);
        stralloc_cats(&fn, "memoryfile.xml");
        stralloc_nul(&fn);
        strlist_push_unique(args, fn.s);
      }
    }
  }

  stralloc_free(&fn);
}
