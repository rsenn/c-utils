#include "xc8.h"
#include "genmakefile.h"
#include "matrix.h"
#include "compiler.h"
#include "../../lib/env.h"
#include "../../lib/arena.h"

strarray xc8_cflags, xc8_ldflags;
const char *xc8_pack, *xc8_version, *xc8_program;
int xc8_universal_opt = -1;

int
xc8_is_cc(void) {
  return tools.compiler && str_start(tools.compiler, "xc8-cc");
}

/* 0x2000, 2000 and 0X2000 as 0x2000; 0 when there is no offset. The
   offset of --code-offset, else the one of an imported -mcodeoffset= flag */
static int
code_offset_opt(stralloc* out, int from_flags) {
  unsigned long v = 0;
  const char* s = 0;
  char** p;

  if(matrix_offset)
    s = matrix_offset;
  else if(from_flags)
    strarray_foreach(&xc8_cflags, p) if(str_start(*p, "-mcodeoffset=")) s = *p + 13;

  if(!s)
    return 0;

  if(str_start(s, "0x") || str_start(s, "0X"))
    s += 2;

  scan_xlong(s, &v);

  if(!v)
    return 0;

  stralloc_copys(out, "0x");
  stralloc_cats(out, s);
  stralloc_nul(out);
  return 1;
}

static int
code_offset(stralloc* out) {
  return code_offset_opt(out, 0);
}

/* one hex for both: the bootloader skips what lies below the offset, so
   GOTO stubs there (ihextool vectors) make the same file run without it */
static int
universal(void) {
  stralloc off;
  const char* c = cfg.chip.s;
  int ret;

  if(!(xc8_is_cc() || (tools.compiler && str_start(tools.compiler, "xc8"))) || xc8_universal_opt == 0)
    return 0;

  if(!c || !cfg.chip.len)
    return 0;

  if(str_start(c, "pic") || str_start(c, "PIC"))
    c += 3;

  if(c[0] != '1' || c[1] != '8')
    return 0;

  stralloc_init(&off);
  ret = code_offset_opt(&off, 1);
  stralloc_free(&off);
  return ret;
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

  stralloc_free(&off);
}

/* the --ldflag options come after the directories are known: $(BUILDDIR) in them is the build directory */
void
xc8_apply_late(void) {
  char** p;

  if(!xc8_is_cc())
    return;

  strarray_foreach(&xc8_ldflags, p) {
    stralloc v, dir;

    stralloc_init(&v);
    stralloc_init(&dir);
    stralloc_copys(&v, *p);

    if(build_tool == TOOL_NINJA) {
      stralloc_cats(&dir, "$(objdir)/");
    } else if(str_equal(tools.make, "gmake")) {
      stralloc_cats(&dir, "$(BUILDDIR)");
    } else {
      /* a script has the absolute directory, a make the one relative to its file */
      stralloc_copy(&dir, &dirs.obj.sa);

      if(build_tool != TOOL_SHELL && build_tool != TOOL_BATCH) {
        stralloc abs;

        stralloc_init(&abs);
        path_abs_sa(&abs, dirs.work.sa.s, dirs.work.sa.len);
        path_rel_sa(&dir, dirs.obj.sa.s, dirs.obj.sa.len, abs.s, abs.len);
        stralloc_free(&abs);
      }

      stralloc_catc(&dir, '/');

      if(build_tool == TOOL_BATCH)
        stralloc_replacec(&dir, '/', '\\');
    }

    stralloc_nul(&dir);
    stralloc_nul(&v);
    stralloc_replaces(&v, "$(BUILDDIR)", dir.s);
    stralloc_nul(&v);
    var_push("LDFLAGS", v.s);
    stralloc_free(&v);
    stralloc_free(&dir);
  }
}

/* directory DIR/vVERSION/bin of a root, when it holds xc8-cc */
static int
find_bin(stralloc* out) {
  static const char* roots[] = {"/opt/microchip/xc8",
                                "/mnt/data/opt/microchip/xc8",
                                "/usr/local/microchip/xc8",
                                "C:/Program Files/Microchip/xc8",
                                "C:/Program Files (x86)/Microchip/xc8",
                                0};
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

    if(path_exists(exe.s) || (stralloc_cats(&exe, ".exe"), stralloc_nul(&exe), path_exists(exe.s))) {
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
  static const char* bases[] = {"/opt/microchip/mplabx", "/mnt/data/opt/microchip/mplabx", "C:/Program Files/Microchip/MPLABX", 0};
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

/* '/' to '\\' for the batch backend */
static void
backslash(stralloc* sa) {
  stralloc_replacec(sa, '/', '\\');
  stralloc_nul(sa);
}

void
xc8_preamble(buffer* out) {
  stralloc bin, dfp, off;
  int gmake, have_bin, have_dfp, uni;

  if(!(xc8_is_cc() || universal()))
    return;

  gmake = str_equal(tools.make, "gmake");
  uni = universal();

  stralloc_init(&bin);
  stralloc_init(&dfp);
  stralloc_init(&off);

  have_bin = find_bin(&bin);
  have_dfp = find_dfp(&dfp);
  code_offset_opt(&off, 1);
  stralloc_nul(&bin);
  stralloc_nul(&dfp);

  if(build_tool == TOOL_NINJA) {
    /* ninja has no ?=; the shell that runs the command takes the value from the environment, as for $$DFP_DIR */
    if(xc8_is_cc()) {
      stralloc v;

      stralloc_init(&v);

      if(have_bin) {
        stralloc_catm(&v, "PATH=$${XC8_BIN:-", bin.s, "}:$$PATH xc8-cc");
        stralloc_nul(&v);
        var_set("CC", v.s);
      }

      stralloc_zero(&v);
      stralloc_catm(&v, "$${DFP_DIR", have_dfp ? ":-" : "", have_dfp ? dfp.s : "", "}/xc8");
      stralloc_nul(&v);
      var_set("MDFP", v.s);
      stralloc_free(&v);
    }
  } else {
    if(build_tool == TOOL_BATCH) {
      backslash(&bin);
      backslash(&dfp);
    }

    if(have_bin) {
      output_default(out, build_tool, "XC8_BIN", bin.s);

      if(gmake)
        buffer_putm_internal(out, "export PATH := $(XC8_BIN):$(PATH)", newline, NULL);
      else if(build_tool == TOOL_SHELL)
        buffer_putm_internal(out, "export PATH=\"$XC8_BIN:$PATH\"", newline, NULL);
      else if(build_tool == TOOL_BATCH)
        buffer_putm_internal(out, "SET PATH=%XC8_BIN%;%PATH%", newline, NULL);
    }

    /* the -mdfp option names the pack, there is no default for it otherwise */
    if(have_dfp) {
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
    } else if(build_tool == TOOL_SHELL) {
      buffer_putm_internal(out,
                           "[ -n \"$DFP_DIR\" ] || echo \"warning: DFP_DIR is not set (device family pack ",
                           xc8_pack ? xc8_pack : "Microchip/<pack>/<version>",
                           ")\" >&2",
                           newline,
                           NULL);
    } else if(build_tool == TOOL_BATCH) {
      buffer_putm_internal(out,
                           "IF NOT DEFINED DFP_DIR ECHO warning: DFP_DIR is not set (device family pack ",
                           xc8_pack ? xc8_pack : "Microchip/<pack>/<version>",
                           ")",
                           newline,
                           NULL);
    }

    if(uni) {
      output_default(out, build_tool, "IHEXTOOL", "ihextool");
      output_default(out, build_tool, "CODE_OFFSET", off.s);
    }

    buffer_putnl(out, 0);
    buffer_flush(out);
  }

  stralloc_free(&bin);
  stralloc_free(&dfp);
  stralloc_free(&off);
}

#define VECTORS_CMD "$(IHEXTOOL) vectors -a $(CODE_OFFSET) -o $@ $<"

/* the rule that writes NAME-universal.hex: ninja names a rule for it */
int
xc8_is_vectors_rule(const target* rule) {
  return rule->recipe.s && !str_diffn(rule->recipe.s, "$(IHEXTOOL) vectors", 19);
}

/* ninja: the rule of the universal hex; the stubs are added with ihextool */
void
xc8_ninja_rules(buffer* out) {
  stralloc off;

  if(!universal() || build_tool != TOOL_NINJA)
    return;

  stralloc_init(&off);
  code_offset_opt(&off, 1);
  buffer_putm_internal(out,
                       "rule ihextool_vectors\n  command = $${IHEXTOOL:-ihextool} vectors -a $${CODE_OFFSET:-",
                       off.s,
                       "} -o $out $in\n",
                       NULL);
  buffer_flush(out);
  stralloc_free(&off);
}

void
xc8_hex_rules(void) {
  MAP_PAIR_T t;
  strlist images;
  const char* x;
  size_t n;
  int uni = universal(), script = build_tool == TOOL_SHELL || build_tool == TOOL_BATCH;

  if(!exts.bin || !((xc8_is_cc() && str_equal(exts.bin, ".elf")) || (uni && str_equal(exts.bin, ".cof"))))
    return;

  strlist_init(&images, '\0');

  MAP_FOREACH(rule_map, t) {
    target* rule = MAP_ITER_VALUE(t);
    set_iterator_t it;

    if(rule->phony || !set_size(&rule->prereq) || !rule->recipe.s)
      continue;

    set_foreach(&rule->output, it, x, n) {
      if(n > 4 && !byte_diff(x + n - 4, 4, exts.bin))
        strlist_pushb_unique(&images, x, n);
    }
  }

  {
    target* all = rule_get("all");
    char* img;

    strlist_foreach_s(&images, img) {
      stralloc hex, un;
      target* rule;

      stralloc_init(&hex);
      stralloc_init(&un);
      stralloc_catb(&hex, img, str_len(img) - 4);
      stralloc_copy(&un, &hex);
      stralloc_cats(&hex, ".hex");
      stralloc_nul(&hex);

      /* the driver writes the .hex when it links the image; a script has no rule to show for it */
      if(!script) {
        rule = rule_get_sa(&hex);
        set_adds(&rule->prereq, img);
        add_path_b(&all->prereq, hex.s, hex.len);
      }

      if(uni) {
        stralloc_cats(&un, "-universal.hex");
        stralloc_nul(&un);
        rule = rule_get_sa(&un);

        if(script) {
          /* the input is named, there is no rule for the .hex between */
          set_adds(&rule->prereq, img);
          stralloc_copys(&rule->recipe, "$(IHEXTOOL) vectors -a $(CODE_OFFSET) -o $@ ");
          stralloc_cat(&rule->recipe, &hex);

          if(build_tool == TOOL_BATCH)
            stralloc_replacec(&rule->recipe, '/', '\\');
        } else {
          set_adds(&rule->prereq, hex.s);
          stralloc_copys(&rule->recipe, VECTORS_CMD);
        }

        stralloc_nul(&rule->recipe);
        add_path_b(&all->prereq, un.s, un.len);
      }

      stralloc_free(&hex);
      stralloc_free(&un);
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
