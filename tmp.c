/**
 * @brief sourcedir_add
 * @param list
 * @param path
 */
void
sourcedir_add(strlist* list, const char* path) {
  if(srcdir.len && !stralloc_equals(&srcdir, ".")) {
    stralloc sa;
    stralloc_init(&sa);

    stralloc_copy(&sa, &srcdir);
    if(!stralloc_endb(&srcdir, &pathsep_make, 1))
      stralloc_catc(&sa, pathsep_make);
    stralloc_cats(&sa, path);

    strlist_push_unique_sa(list, &sa);
    stralloc_free(&sa);
  } else {
    strlist_push_unique(list, path);
  }
}
/**
 * @brief sourcedir_compile_rules  Generate compile rules for every source file given
 * @param rules                     All rules
 * @param srcdir                    source dir structure
 * @param dir                       source dir path
 * @return
 */
target*
sourcedir_compile_rules(HMAP_DB* rules, sourcedir* sdir, const char* dir) {
  sourcefile* src;
  target* rule;
  stralloc target, srcs, obj;
  size_t len;

  stralloc_init(&target);
  path_output("%", &target, exts.obj);
  stralloc_cats(&target, ": ");

  stralloc_init(&srcs);

  len = target.len;

  stralloc_init(&obj);

  slink_foreach(&sdir->sources, src) {
    const char* ext = 0;

    if(!src->name)
      continue;

    if(!is_source(src->name))
      continue;

    // s = str_basename(src->name);

    target.len = len;

    stralloc_zero(&srcs);
    path_prefix_s(&srcdir, src->name, &srcs);
    path_wildcard(&srcs, "%");
    stralloc_replacec(&srcs, pathsep_make == '/' ? '\\' : '/', pathsep_make);

    stralloc_zero(&obj);
    path_output(src->name, &obj, exts.obj);

    if(str_start(tools.make, "g") || ((shell | batch) && batchmode)) {
      stralloc_cat(&target, &srcs);
      // path_wildcard(&target, "%");
    } else if(batchmode) {
      stralloc_zero(&target);
      stralloc_catm_internal(&target, "{", dir, "}", ext, "{", dirs.work.sa.s, "}", exts.obj, ":", 0);
    } else {
    }
    stralloc_replacec(&target, pathsep_make == '/' ? '\\' : '/', pathsep_make);

    if((rule = rule_get_sa(&target))) {

      if(rule->recipe.s == 0) {
        strlist_zero(&rule->output);
        strlist_zero(&rule->prereq);
      }

      add_path_sa(&rule->output, &obj);

      // sourcedir_add(&rule->prereq, srcs.s);

      if(rule->recipe.s)
        continue;

      if((shell | batch) == 0 && batchmode) {
        // rule->recipe = malloc(sizeof(stralloc));
        stralloc_init(&rule->recipe);
        stralloc_copy(&rule->recipe, &compile_command);
        stralloc_replaces(&rule->recipe, "-Fo", "-Fd");
        stralloc_replaces(&rule->recipe, "$@", dirs.work.sa.s);

      } else {
        size_t p, e;
        char* x;

        if(dirs.work.sa.len == 0 || stralloc_equals(&dirs.work.sa, ".")) {
          stralloc_copy(&rule->recipe, &compile_command);
          x = stralloc_begin(&rule->recipe);
          // e = stralloc_end(&rule->recipe);

          p = e = stralloc_finds(&rule->recipe, "$@");
          while(p > 0 && !((x[p + 1] == '/' || x[p + 1] == '-') && x[p] == ' ')) --p;

          e += 2;
          if(x[e] == '"')
            ++e;

          stralloc_remove(&rule->recipe, p, e - p);
        } else {
          stralloc_weak(&rule->recipe, &compile_command);
        }

        //        rule->recipe = /* str_start(tools.make, "g") ? NULL :*/ &compile_command;
      }
    }
  }

  stralloc_free(&obj);
  stralloc_free(&target);

#ifdef DEBUG_OUTPUT
  rule_dump(rule);
#endif

  return rule;
}

/**
 * @brief sourcedir_dump_all
 * @param b
 * @param sourcedirs
 */
void
sourcedir_dump_all(buffer* b, HMAP_DB* sourcedirs) {
  TUPLE* t;

  MAP_FOREACH(sourcedirs, t) {
    sourcedir* srcdir = hmap_data(t);
    sourcefile* pfile;

    buffer_puts(b, " '");
    buffer_put(b, t->key, t->key_len);
    buffer_puts(b, "' (");
    buffer_putulong(b, srcdir->n_sources);
    buffer_puts(b, "): [");

    slink_foreach(srcdir->sources, pfile) {
      buffer_putspace(b);
      buffer_puts(b, pfile->name);
    }
    buffer_puts(b, " ]");

    buffer_putnlflush(b);
  }
}

/**
 * @brief sourcedir_find  Searches for a source directory
 * @param path           Path string
 * @return               Pointer to sourcedir structure or NULL
 */
sourcedir*
sourcedir_find(const char* path) {
  return MAP_GET(sourcedirs, path, str_len(path) + 1);
}

/**
 * @brief sourcedir_findb  Searches for a source directory
 * @param x                Path buffer
 * @param n                Length of path
 * @return               Pointer to sourcedir structure or NULL
 */
sourcedir*
sourcedir_findb(const char* x, size_t n) {
  sourcedir* ret;
  stralloc p;
  stralloc_init(&p);
  stralloc_copyb(&p, x, n);
  ret = sourcedir_find_sa(&p);
  stralloc_free(&p);
  return ret;
}

/**
 * @brief sourcedir_find_sa Searches for a source directory
 * @param path             Path stralloc
 * @return                 Pointer to sourcedir structure or NULL
 */
sourcedir*
sourcedir_find_sa(stralloc* path) {
  stralloc_nul(path);
  return MAP_GET(sourcedirs, path->s, path->len + 1);
}

/**
 * @brief sourcedir_gen_rule
 * @param rules                     All rules
 * @param srcdir                    source dir structure
 * @param name                      source dir name
 */
void
sourcedir_gen_rule(HMAP_DB* rules, sourcedir* sdir, const char* name) {
  sourcefile* src;
  target* rule;
  stralloc mask;
  stralloc_init(&mask);

  slink_foreach(&sdir->sources, src) {

    const char* s;
    debug_s("sourcefile", src->name);

    if(!is_source(src->name))
      continue;

    s = str_ndup(src->name, str_rchr(src->name, '.'));

    stralloc_zero(&mask);
    path_prefix_s(&dirs.work.sa, str_basename(s), &mask);
    stralloc_cats(&mask, exts.obj);

    if(batchmode || str_start(tools.make, "g")) {
      stralloc_cats(&mask, ": ");
      stralloc_cats(&mask, src->name);

      path_wildcard(&mask, "%");
    }
    // debug_sa("mask", &mask);

    if((rule = rule_get_sa(&mask))) {
      strlist_push(&rule->prereq, src->name);

      if(rule->recipe.s == 0) {
        stralloc_weak(&rule->recipe, &compile_command);
      }
    }

    free((void*)s);
  }
  stralloc_free(&mask);
}

srcdir = sourcedir_get_sa(&dir);
sourcedir*
sourcedir_get_sa(stralloc* path) {
  sourcedir* s;
  stralloc_nul(path);

  if(!(s = sourcedir_find_sa(path))) {
    sourcedir newdir;
    byte_zero(&newdir, sizeof(newdir));
    hmap_set(&sourcedirs, path->s, path->len + 1, &newdir, sizeof(newdir));
    s = hmap_get(sourcedirs, path->s, path->len + 1);
  }
  return s;
}

/**
 * @brief sourcedir_lib_rule  Generate lib rule for source dir
 * @param rules                     All rules
 * @param srcdir                    source dir structure
 * @param dir                       source dir path
 * @return
 */
target*
sourcedir_lib_rule(HMAP_DB* rules, sourcedir* srcdir, const char* name) {
  target *dep = 0, *rule;
  stralloc sa;
  stralloc_init(&sa);

  path_prefix_s(&dirs.work.sa, name, &sa);
  // stralloc_copys(&sa, name);

  stralloc_cats(&sa, exts.lib);

  // debug_sa("sourcedir_lib_rule", &sa);

  if((str_start(tools.make, "g") || batchmode) && cfg.mach.arch != PIC) {
    dep = sourcedir_compile_rules(rules, srcdir, name);
  } else {
    /* if(0 && tools.preproc) {
       gen_simple_compile_rules(rules, srcdir, name, ".c", exts.pps, &preprocess_command);
       dep = gen_simple_compile_rules(rules, srcdir, name, exts.pps, exts.obj, &compile_command);
     } else {*/
    dep = gen_simple_compile_rules(rules, srcdir, name, ".c", exts.obj, &compile_command);
    //}
  }

  if((rule = rule_get_sa(&sa))) {
    sourcefile* pfile;
    strlist_init(&rule->prereq, ' ');

    if(dep) {
      size_t n;
      char* s;
      strlist_cat(&rule->prereq, &dep->output);

      array_catb(&rule->objs, &dep, sizeof(target*));

    } else {
      slink_foreach(srcdir->sources, pfile) {
        if(!str_end(pfile->name, ".c"))
          continue;
        stralloc_zero(&sa);

        if(vpath.sa.len)
          path_extension(pfile->name, &sa, exts.obj);
        else

          path_output(pfile->name, &sa, exts.obj);

        add_path_sa(&rule->prereq, &sa);
      }
    }

    stralloc_weak(&rule->recipe, &lib_command);

#ifdef DEBUG_OUTPUT
    rule_dump(rule);
#endif
  }

  stralloc_free(&sa);
  return rule;
}

/**
 * @brief sourcedir_populate  Creates a hash-map of all source directories
 * @param sources
 * @param sourcedirs
 */
void
sourcedir_populate(HMAP_DB* sourcedirs, strarray* sources) {
  const char** p;
  stralloc dir;
  strlist l;
  stralloc_init(&dir);
  strlist_init(&l, '\0');

  strarray_foreach(sources, p) {
    size_t n;
    const char* x;
    const char* s;
    size_t dlen;
    sourcedir* srcdir;
    sourcefile* file = sources_new(*p);
    stralloc r;
    stralloc_init(&r);

    strlist_zero(&l);

    if((x = mmap_read(*p, &n)) != 0) {
      includes_extract(x, n, &l, 0);
      extract_pptok(x, n, &pptoks);
      mmap_unmap(x, n);
    } else {
      buffer_putm_internal(buffer_2, "ERROR opening '", *p, "'\n", 0);
      buffer_putnlflush(buffer_2);
    }

    path_dirname(*p, &dir);
    stralloc_nul(&dir);
    dlen = dir.len;
    // debug_sa("path_dirname(srcfile)", &dir);

    srcdir = sourcedir_get_sa(&dir);
    slist_add(&srcdir->sources, &file->link);
    ++srcdir->n_sources;

    stralloc_replacec(&l.sa, PATHSEP_C == '\\' ? '/' : '\\', PATHSEP_C);

    strlist_foreach_s(&l, s) {
      dir.len = dlen;
      stralloc_catc(&dir, PATHSEP_C);
      stralloc_cats(&dir, s);
      stralloc_nul(&dir);

      stralloc_zero(&r);
      path_collapse(dir.s, &r);
      strlist_push_unique_sa(&srcdir->includes, &r);
    }

    dir.len = dlen;
    // debug_sa("srcdir", &dir);
    // debug_sa("includes", &srcdir->includes);

    stralloc_free(&r);
    strlist_free(&l);
  }

  stralloc_free(&dir);
}
