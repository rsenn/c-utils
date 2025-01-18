#include "generate.h"
#include "ansi.h"
#include "../../debug.h"
#include "../../genmakefile.h"

strarray bins = {0}, progs = {0};
stralloc output_name = {0};

static inline int
filter_pptoks(const void* x, size_t n) {
  return byte_equal(x, n > 4 ? 4 : n, "USE_");
}

/**
 * Add a path to a strlist
 */
static void
add_path_b(set_t* s, const char* path, size_t len) {
  if(set_has(s, path, len))
    return;

  set_insert(s, path, len);
}

/**
 * Add a path to a strlist
 */
static void
add_path(set_t* s, const char* path) {
  add_path_b(s, path, str_len(path));
}

/**
 * @brief add_srcpath
 * @param list
 * @param path
 */
static void
add_srcpath(set_t* s, const char* path, char psm) {
  stralloc sa;
  stralloc_init(&sa);

  if(sources_dir.len && !stralloc_equals(&sources_dir, ".")) {
    stralloc_zero(&sa);
    path_prefix_s(&sources_dir, path, &sa, psm);
    set_addsa(s, &sa);
    stralloc_free(&sa);
  } else {
    set_adds(s, path);
  }

  stralloc_free(&sa);
}

static void
add_source(set_t* s, const char* path, char psm) {
  stralloc sa;
  stralloc_init(&sa);

  if(sources_dir.len && !stralloc_equals(&sources_dir, ".")) {
    stralloc_zero(&sa);
    path_prefix_s(&sources_dir, path, &sa, psm);
    set_addsa(s, &sa);
  } else {
    set_adds(s, path);
  }

  stralloc_free(&sa);
}

/**
 * @brief add_path_sa
 * @param list
 * @param path
 */
void
add_path_sa(set_t* s, stralloc* path) {
  add_path_b(s, path->s, path->len);
}

target*
generate_single_rule(stralloc* output, stralloc* cmd) {
  target* rule;

  if((rule = rule_get_sa(output))) {
    set_init(&rule->prereq, 0);

    if(cmd && !(rule->recipe.len && stralloc_equal(&rule->recipe, cmd))) {
      if(rule->recipe.len)
        stralloc_cats(&rule->recipe, "\n");

      stralloc_cat(&rule->recipe, cmd);
    }
  }

  return rule;
}

/**
 * @brief generate_clean_rule  Generate clean rule which removes all target outputs
 * @param rules
 */
void
generate_clean_rule(char psm) {
  target* clean = rule_get("clean");
  MAP_PAIR_T t;
  char* arg;
  size_t cmdoffs, lineoffs = 0;
  stralloc fn;
  strlist delete_args;

  stralloc_init(&fn);
  strlist_init(&delete_args, '\0');

  if(commands.delete.len == 0)
    stralloc_copys(&commands.delete, cfg.sys.type == NTOS ? "DEL /F /Q" : "$(RM)");

  cmdoffs = commands.delete.len;

  MAP_FOREACH(rules, t) {
    target* rule = MAP_ITER_VALUE(t);
    const char* name = MAP_ITER_KEY(t);

    if(rule->phony)
      continue;

    if(name[str_chr(name, '%')]) {
      stralloc_catset(&delete_args.sa, &rule->output, " ");
      continue;
    }

    /* If the rule has prerequisites  and a recipe, it must be a producing rule */
    if(set_size(&rule->prereq) && rule->recipe.s) {
      size_t bpos;

      if((MAP_ITER_KEY(t))[(bpos = str_rchr(MAP_ITER_KEY(t), '{'))]) {
        size_t epos = str_rchr(&(MAP_ITER_KEY(t))[bpos + 1], '}');
        stralloc_zero(&fn);
        stralloc_catb(&fn, &(MAP_ITER_KEY(t)[bpos + 1]), epos);
        stralloc_catc(&fn, psm);
        stralloc_cats(&fn, "*");
        stralloc_catb(&fn, &(MAP_ITER_KEY(t)[bpos + 1 + epos + 1]), str_chr(&(MAP_ITER_KEY(t)[bpos + 1 + epos + 1]), ':'));
        stralloc_nul(&fn);
        arg = fn.s;
      } else {
        stralloc_copys(&fn, name);
        /* If possible, transform file name into a wildcard pattern */
        arg = path_wildcard(&fn, "*");
      }

      /* Add to deletion list */
      strlist_push_unique(&delete_args, arg);
    }
  }

  strlist_nul(&delete_args);

  strlist_foreach_s(&delete_args, arg) {
    if(commands.delete.len - lineoffs + str_len(arg) >= MAX_CMD_LEN) {
      stralloc_readyplus(&commands.delete, cmdoffs + 3);
      stralloc_catm_internal(&commands.delete, "\n\t", NULL);
      stralloc_catb(&commands.delete, commands.delete.s, cmdoffs);
      lineoffs = commands.delete.len;
    }

    stralloc_catc(&commands.delete, ' ');
    stralloc_cats(&commands.delete, arg);

    if(arg[str_chr(arg, '*')])
      lineoffs = -MAX_CMD_LEN;
  }

  stralloc_weak(&clean->recipe, &commands.delete);

  strlist_free(&delete_args);
  stralloc_free(&fn);
}

target*
generate_mkdir_rule(stralloc* dir) {
  target* rule = 0;
  if(stralloc_length(dir) && !stralloc_equals(dir, ".")) {
    if((rule = rule_get_sa(dir))) {
      stralloc_weak(&rule->recipe, &commands.mkdir);
    }
  }
  return rule;
}

/**
 * @brief generate_srcdir_compile_rules Generate compile rules for every source file given
 * @param rules                     All rules
 * @param srcdir source dir structure
 * @param dir source dir path
 * @return
 */
target*
generate_srcdir_compile_rules(sourcedir* srcdir, const char* dir, bool shell, bool batch, bool batchmode, char psa, char psm) {
  sourcefile* src;
  target* rule = 0;
  MAP_PAIR_T t;
  stralloc target, source, obj, defines;
  size_t len, tlen;
  strlist pptoks;
  const char* tok;
  set_iterator_t it;

  stralloc_init(&target);
  path_output("%", &target, exts.obj, psa);
  stralloc_cats(&target, ": ");
  stralloc_init(&source);
  tlen = target.len;
  stralloc_init(&obj);
  strlist_init(&pptoks, '\0');

  set_foreach(&srcdir->pptoks, it, tok, len) {
    if(filter_pptoks(tok, len))
      strlist_pushb(&pptoks, tok, len);
  }

#ifdef DEBUG_OUTPUT_
  buffer_puts(buffer_2, "pptoks: ");
  strlist_dump(buffer_2, &pptoks);
  buffer_flush(buffer_2);
#endif

  strlist_free(&pptoks);
  stralloc_init(&defines);

  if(set_size(&srcdir->pptoks) > 0) {
    stralloc_copys(&defines, "DEFS +=");

    set_foreach(&srcdir->pptoks, it, tok, len) {
      if(filter_pptoks(tok, len)) {
        stralloc_cats(&defines, " -D");
        stralloc_catb(&defines, tok, len);
        stralloc_cats(&defines, "=1");
      }
    }
  }

  slist_foreach(srcdir->sources, src) {
    const char* ext = 0;

#ifdef DEBUG_OUTPUT
    debug_str("Adding source", src->name);
#endif

    if(!is_source(src->name))
      continue;

    target.len = tlen;
    stralloc_zero(&source);
    path_prefix_s(&sources_dir, src->name, &source, psm);
    path_wildcard(&source, "%");
    stralloc_replacec(&source, psm == '/' ? '\\' : '/', psm);
    stralloc_zero(&obj);
    path_output(src->name, &obj, exts.obj, psa);

    if(str_start(tools.make, "g") || ((shell | batch) && batchmode)) {
      stralloc_cat(&target, &source);
    } else if(batchmode) {
      stralloc_zero(&target);
      stralloc_catm_internal(&target, "{", dir, "}", ext, "{", dirs.work.sa.s, "}", exts.obj, ":", NULL);
    } else {
    }

    if(!rule)
      rule = rule_get_sa(&target);

    if(rule) {
      stralloc_copy(&rule->vars.sa, &defines);

      if(rule->recipe.s == 0) {
        set_clear(&rule->output);
        set_clear(&rule->prereq);
      }

      set_addsa(&rule->output, &obj);
      add_srcpath(&rule->prereq, source.s, psm);

      if(rule->recipe.s)
        continue;

      if((shell | batch) == 0 && batchmode) {
        stralloc_init(&rule->recipe);
        stralloc_copy(&rule->recipe, &commands.compile);
        stralloc_replaces(&rule->recipe, "-Fo", "-Fd");
        stralloc_replaces(&rule->recipe, "$@", dirs.work.sa.s);
        continue;
      }

      {
        size_t p, e;
        char* x;

        if(dirs.work.sa.len == 0 || stralloc_equals(&dirs.work.sa, ".")) {
          stralloc_copy(&rule->recipe, &commands.compile);
          x = stralloc_begin(&rule->recipe);
          p = e = stralloc_finds(&rule->recipe, "$@");
          while(p > 0 && !((x[p + 1] == '/' || x[p + 1] == '-') && x[p] == ' '))
            p--;
          e += 2;
          if(x[e] == '"')
            e++;
          stralloc_remove(&rule->recipe, p, e - p);
        } else {
          stralloc_weak(&rule->recipe, &commands.compile);
        }
      }
    }
  }

  strlist_free(&pptoks);
  stralloc_free(&obj);
  stralloc_free(&source);
  stralloc_free(&target);
  stralloc_free(&defines);

  return rule;
}

/**
 * @brief generate_simple_compile_rules  compile rules for every  source file in srcdir
 * @param rules                     All  rules
 * @param srcdir source dir structure
 * @param dir source dir path
 * @return
 */
target*
generate_simple_compile_rules(sourcedir* srcdir, const char* dir, const char* fromext, const char* toext, stralloc* cmd, char psa) {
  sourcefile* src;
  stralloc ppsrc, obj;

  stralloc_init(&ppsrc);
  stralloc_init(&obj);

#ifdef DEBUG_OUTPUT_
  buffer_putm_internal(buffer_2, "generate_simple_compile_rules '", dir, "' ", fromext, " ", toext, NULL);
  buffer_putnlflush(buffer_2);
#endif

  slist_foreach(srcdir->sources, src) {
    target* rule;
    const char *base, *srcname = src->name;

    if(srcname == 0)
      continue;

    base = str_basename(srcname);

    if(!str_equal(base + str_rchr(base, '.'), fromext))
      continue;

    if(tools.preproc) {
      stralloc_zero(&obj);
      path_output(base, &obj, ".pp.c", psa);

      if((rule = rule_get_sa(&obj))) {
        add_source(&rule->prereq, src->name, psa);
        stralloc_copy(&rule->recipe, &commands.preprocess);
        ppsrc = obj;
        srcname = ppsrc.s;
      }
    }

    stralloc_init(&obj);
    path_output(base, &obj, toext, psa);

    if((rule = rule_get_sa(&obj))) {
      add_source(&rule->prereq, srcname, psa);

      if(rule->recipe.s == NULL) {
        stralloc_weak(&rule->recipe, cmd);
        array_catb(&srcdir->rules, &rule, sizeof(target*));
      }
    }
  }

  stralloc_free(&obj);
  stralloc_free(&ppsrc);

  return 0;
}

/**
 * @brief generate_srcdir_lib_rule  Generate  lib rule for source dir
 * @param rules                     All rules
 * @param srcdir source dir structure
 * @param dir source dir path
 * @return
 */
target*
generate_srcdir_lib_rule(sourcedir* srcdir, const char* name, bool shell, bool batch, bool batchmode, char psa, char psm) {
  target *dep = 0, *rule;
  stralloc sa;
  stralloc_init(&sa);
  path_prefix_s(&dirs.work.sa, name, &sa, psm);
  stralloc_cats(&sa, exts.lib);

#ifdef DEBUG_OUTPUT_
  debug_str("generate_srcdir_lib_rule", name);
  debug_sa("generate_srcdir_lib_rule", &sa);
#endif

  if((str_start(tools.make, "g") || batchmode) && cfg.mach.arch != PIC) {
    buffer_putm_internal(buffer_2, "generate_srcdir_compile_rules: ", name, NULL);
    buffer_flush(buffer_2);
    dep = generate_srcdir_compile_rules(srcdir, name, shell, batch, batchmode, psa, psm);
  } else {
    dep = generate_simple_compile_rules(srcdir, name, exts.src, exts.obj, &commands.compile, psa);
  }

  if((rule = rule_get_sa(&sa))) {
    sourcefile* pfile;
    set_init(&rule->prereq, 0);

    if(dep) {
      set_cat(&rule->prereq, &dep->output);
      array_catb(&rule->objs, &dep, sizeof(target*));
    } else {
      slist_foreach(srcdir->sources, pfile) {

        if(pfile->name == NULL || !is_source(pfile->name))
          continue;

        stralloc_zero(&sa);

        if(vpath.sa.len)
          path_extension(pfile->name, &sa, exts.obj);
        else
          path_output(pfile->name, &sa, exts.obj, psa);

        set_addsa(&rule->prereq, &sa);
      }
    }

    stralloc_weak(&rule->recipe, &commands.lib);
  }

  stralloc_free(&sa);

  return rule;
}

/**
 * @brief generate_srcdir_rule
 * @param rules                     All rules
 * @param srcdir source dir structure
 * @param name source dir name
 */
void
generate_srcdir_rule(sourcedir* sdir, const char* name, bool batchmode, char psm) {
  sourcefile* src;
  target* rule;
  stralloc mask;
  stralloc_init(&mask);

  slist_foreach(sdir->sources, src) {
    const char* s;

#ifdef DEBUG_OUTPUT_
    debug_str("sourcefile", src->name);
#endif

    if(!is_source(src->name))
      continue;

    s = str_ndup(src->name, str_rchr(src->name, '.'));
    stralloc_zero(&mask);
    path_prefix_s(&dirs.work.sa, str_basename(s), &mask, psm);
    stralloc_cats(&mask, exts.obj);

    if(batchmode || str_start(tools.make, "g")) {
      stralloc_cats(&mask, ": ");
      stralloc_cats(&mask, src->name);
      path_wildcard(&mask, "%");
    }

    if((rule = rule_get_sa(&mask))) {
      set_adds(&rule->prereq, src->name);

      if(rule->recipe.s == 0)
        stralloc_weak(&rule->recipe, &commands.compile);
    }

    free((void*)s);
  }

  stralloc_free(&mask);
}

/**
 * @brief generate_lib_rules  Generate library rules for every source dir
 * @param rules
 * @param srcdirs
 */
void
generate_lib_rules(bool shell, bool batch, bool batchmode, char psa, char psm) {
  MAP_PAIR_T t;
  target* all;
  stralloc inc, abspath;
  stralloc_init(&inc);
  stralloc_init(&abspath);
  all = rule_get("all");

  MAP_FOREACH(srcdir_map, t) {
    target* rule;
    sourcedir* srcdir = *(sourcedir**)MAP_ITER_VALUE(t);
    const char* base = path_basename(MAP_ITER_KEY(t));

    if(str_equal(base, ".")) {
      stralloc_zero(&abspath);
      path_absolute(MAP_ITER_KEY(t), &abspath);
      stralloc_nul(&abspath);
      base = path_basename(MAP_ITER_KEY(t));
    }

#ifdef DEBUG_OUTPUT_
    debug_str("srcdir", MAP_ITER_KEY(t));
    debug_str("base", base);
#endif

    if(strlist_contains(&build_as_lib, base) || base[0] == '.' || base[0] == '\0')
      continue;

    rule = generate_srcdir_lib_rule(srcdir, base, shell, batch, batchmode, psa, psm);
    set_adds(&link_libraries, rule->name);
    set_adds(&all->prereq, rule->name);
    array_catb(&srcdir->rules, &rule, sizeof(target*));
  }

  stralloc_free(&inc);
  stralloc_free(&abspath);
}

target*
generate_program_rule(const char* filename, char psa) {
  target *preprocess = 0, *compile = 0, *rule = 0, *all = 0;
  const char *x, *link_lib;
  set_t incs = SET(), deps = SET();
  strlist libs;
  stralloc dir, bin, ppsrc, obj;
  sourcedir* srcdir;

  strlist_init(&libs, ' ');
  stralloc_init(&ppsrc);
  stralloc_init(&obj);
  stralloc_init(&dir);
  stralloc_init(&bin);

#ifdef DEBUG_OUTPUT_
  buffer_putm_internal(buffer_2, "[1]", GREEN256, "generate_program_rule(", NC, filename, GREEN256, ") ", NC, 0);
  buffer_putnlflush(buffer_2);
  debug_str("generate_program_rule", filename);
#endif

  path_dirname(filename, &dir);
  srcdir = sourcedir_getsa(&dir);

  if(tools.preproc)
    path_output(filename, &ppsrc, exts.pps, psa);

  path_output(filename, &obj, exts.obj, psa);

  if(tools.preproc && (preprocess = rule_get_sa(&ppsrc))) {
    add_source(&preprocess->prereq, filename, psa);
    stralloc_weak(&preprocess->recipe, &commands.preprocess);
  }

  if((compile = rule_get_sa(&obj))) {
    add_source(&compile->prereq, filename, psa);
    stralloc_weak(&compile->recipe, &commands.compile);
    stralloc_zero(&compile->recipe);

#ifdef DEBUG_OUTPUT
    buffer_putm_internal(buffer_2, "[2]", GREEN256, "generate_program_rule(", NC, filename, GREEN256, ") ", NC, "\033[38;5;82mcompile rule" NC " '", compile->name, "' recipe '", compile->recipe.s, "'", NULL);
    buffer_putnlflush(buffer_2);
#endif
  }

  stralloc_zero(&bin);

  if(!cmd_libs) {
    stralloc outname;
    stralloc_init(&outname);
    stralloc_cats(&outname, path_basename(filename));

    if(stralloc_endb(&outname, exts.src, 2))
      outname.len -= 2;

    stralloc_nul(&outname);
    path_output(outname.s, &bin, exts.bin, psa);
  } else {
    path_extension(obj.s, &bin, exts.bin);
  }

  all = rule_get("all");
  add_path_b(&all->prereq, bin.s, bin.len);

  if((rule = rule_get_sa(&bin))) {
    size_t n;
    sourcefile* pfile;
    set_iterator_t it;
    add_path_sa(&rule->prereq, &obj);

    if(1 || cmd_libs) {
      slist_foreach(srcdir->sources, pfile) {
        if(!pfile->has_main && !is_include(pfile->name)) {
          stralloc_zero(&obj);
          path_output(pfile->name, &obj, exts.obj, psa);
          add_path_sa(&rule->prereq, &obj);
        }
      }
    } else {
      slink* source;

      dlist_foreach_down(&sources_list, source) {
        sourcefile* sfile = dlist_data(source, sourcefile*);
        char* filename = (char*)sfile->name;
        stralloc_zero(&dir);
        path_dirname(filename, &dir);

        if(str_end(filename, exts.inc))
          continue;

        strlist_push_unique_sa(&vpath, &dir);
        stralloc_zero(&obj);
        path_output(filename, &obj, exts.obj, psa);
        add_path_sa(&rule->prereq, &obj);
      }
    }

    stralloc_weak(&rule->recipe, &commands.link);
    stralloc_nul(&rule->recipe);

#ifdef DEBUG_OUTPUT
    buffer_putm_internal(buffer_2, "[3]", GREEN256, "generate_program_rule(", NC, filename, GREEN256, ") ", NC, "link rule" NC " '", rule->name, "' recipe '", rule->recipe.s, "'", NULL);
    buffer_putnlflush(buffer_2);
#endif

    includes_to_libs(&incs, &libs);
    rule_list(&libs, &rule->deps);
    set_clear(&deps);
    rule_dep_list(rule, &deps);

    set_foreach(&link_libraries, it, link_lib, n) {
      const char* s;
      size_t len;
      target* lib = rule_find_b(link_lib, n);
      set_foreach(&lib->prereq, it, s, len) { set_add(&deps, s, len); }
      add_path(&all->prereq, lib->name);
    }

    if(set_size(&deps))
      set_foreach(&deps, it, x, n) { set_add(&rule->prereq, x, n); }
  }

  strlist_free(&libs);
  stralloc_free(&ppsrc);
  stralloc_free(&bin);
  stralloc_free(&obj);
  stralloc_free(&dir);

  return rule;
}

/**
 * @brief generate_link_rules  Generate compile rules for every source file with a main()
 * @param rules
 * @param sources
 * @return
 */
int
generate_link_rules(char psa, char psm) {
  int num_main = 0, count = 0;
  target* link = 0;
  set_t incs, deps;
  strlist libs, indir;
  struct dnode* node;

  set_init(&incs, 0);
  set_init(&deps, 0);
  strlist_init(&libs, ' ');
  strlist_init(&indir, ' ');

#ifdef DEBUG_OUTPUT
  buffer_putm_internal(buffer_2, "[1]", GREEN256, "generate_link_rules(", NC, GREEN256, ") ", NC, 0);
  buffer_putnlflush(buffer_2);
#endif

  dlist_foreach_down(&sources_list, node) {
    sourcefile* file = dlist_data(node, sourcefile*);
    char* filename = (char*)file->name;

#ifdef DEBUG_OUTPUT_
    buffer_putm_internal(buffer_2, "GEN_LINK_RULES file = ", filename, NULL);
    buffer_putnlflush(buffer_2);
#endif

    set_clear(&incs);
    strlist_zero(&libs);
    set_clear(&deps);
    strlist_zero(&indir);

    if(is_source(filename) && main_present(filename)) {
      target* rule;

      if(!link && (rule = generate_program_rule(filename, psa)))
        link = rule;

      strarray_push_unique(&progs, filename);
      num_main++;
    }

    // count++;
  }

  if(num_main <= 1 && link && output_name.len) {
    stralloc oldname;
    stralloc_init(&oldname);
    stralloc_catset(&oldname, &link->output, " ");

    if(!stralloc_endb(&output_name, exts.bin, str_len(exts.bin)))
      stralloc_cats(&output_name, exts.bin);

    stralloc_nul(&output_name);
    rule_rename(link, output_name.s, psm);
  }

  set_free(&incs);
  strlist_free(&libs);
  set_free(&deps);
  strlist_free(&indir);

  return count + num_main;
}

/**
 * @brief generate_install_rules
 * @param rules
 * @return
 */
target*
generate_install_rules() {
  MAP_PAIR_T t;
  target* inst = NULL;
  const char* v = 0;

  MAP_FOREACH(rules, t) {
    target* rule = MAP_ITER_VALUE(t);
    bool do_lib, do_bin;

    do_lib = (inst_libs && (str_end(MAP_ITER_KEY(t), ".lib") || str_end(MAP_ITER_KEY(t), ".a") || MAP_ITER_KEY(t)[str_find(MAP_ITER_KEY(t), ".so")] || rule->recipe.s == commands.lib.s));
    do_bin = (inst_bins && (str_end(MAP_ITER_KEY(t), ".dll") || str_end(MAP_ITER_KEY(t), ".exe") || rule->recipe.s == commands.link.s));

    if(!(do_lib || do_bin))
      continue;

    if(!inst) {
      inst = rule_get("install");
      // inst->recipe = malloc(sizeof(stralloc));

      stralloc_init(&inst->recipe);
      set_adds(&inst->prereq, "all");
    }

    if(!var_isset("prefix")) {
      var_set("prefix", "/usr");
      stralloc_catm_internal(&inst->recipe, newline, "\t$(INSTALL_DIR) $(DESTDIR)$(prefix)", NULL);

      if(!v) {
        v = var_set("INSTALL", "install")->value.sa.s;
        var_set("INSTALL_DIR", str_start(v, "install") ? "$(INSTALL) -d" : "mkdir");

        if(do_lib)
          var_set("INSTALL_DATA", str_start(v, "install") ? "$(INSTALL) -m 644" : "$(INSTALL)");
      }
    }

    if(do_bin) {
      if(!var_isset("bindir")) {
        var_set("bindir", "$(prefix)/bin");
        stralloc_catm_internal(&inst->recipe, newline, "\t$(INSTALL_DIR) $(DESTDIR)$(bindir)", NULL);
      }

      var_set("INSTALL_EXEC", str_start(v, "install") ? "$(INSTALL) -m 755" : "$(INSTALL)");
      stralloc_catm_internal(&inst->recipe, newline, "\t$(INSTALL_EXEC) ", MAP_ITER_KEY(t), " $(DESTDIR)$(bindir)", NULL);
    }

    if(do_lib) {
      if(!var_isset("libdir")) {
        var_set("libdir", "$(prefix)/lib");

        if(str_end(tools.compiler, "64"))
          var_push("libdir", "$(X64)");

        stralloc_catm_internal(&inst->recipe, newline, "\t$(INSTALL_DIR) $(DESTDIR)$(libdir)", NULL);
      }

      stralloc_catm_internal(&inst->recipe, newline, "\t$(INSTALL_DATA) ", MAP_ITER_KEY(t), " $(DESTDIR)$(libdir)", NULL);
    }
  }

  return inst;
}
