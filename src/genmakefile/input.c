#include "input.h"
#include "var.h"
#include "generate.h"
#include "../../lib/path.h"
#include "../../lib/hashmap.h"
#include "../../genmakefile.h"
#include <errno.h>

#define REMOVE(sa, i, x, n) \
  size_t j = n; \
  i = stralloc_findb((sa), x, n); \
\
  if(i < (sa)->len) { \
    int q = i > 0 && (sa)->s[i - 1] == '"'; \
    if(q) { \
      i--; \
      j++; \
      if((sa)->s[i + j] == '"') \
        j++; \
    } \
    if((sa)->s[i + j] == ' ') \
      j++; \
    stralloc_replace((sa), i, j, "", 0); \
  }

static void
libdirs_add(const char* dir) {
  stralloc abs;
  stralloc_init(&abs);

  path_normalize(dir, &abs);

  if(strlist_push_unique_sa(&link_dirs, &abs)) {

#ifdef DEBUG_OUTPUT_
    buffer_puts(buffer_2, "Added to lib dirs: ");
    buffer_putsa(buffer_2, &abs);
    buffer_putnlflush(buffer_2);
#endif
  }

  stralloc_free(&abs);
}

static void
builddir_enter(const char* x, size_t len) {
  stralloc dir;
  stralloc_init(&dir);
  strarray_emplace_sa(&dirstack, &dirs.build.sa);
  stralloc_copyb(&dirs.build.sa, x, len);

  path_relative_b(dirs.build.sa.s, dirs.build.sa.len, &dir);
  set_addsa(&build_directories, &dir);

#ifdef DEBUG_OUTPUT
  buffer_puts(buffer_2, "Entering [");
  buffer_putlong(buffer_2, strarray_size(&dirstack));
  buffer_puts(buffer_2, "] '");
  buffer_putsa(buffer_2, &dir);
  buffer_puts(buffer_2, "'");
  buffer_putnlflush(buffer_2);
#endif

  stralloc_free(&dir);
}

static void
builddir_leave(const char* x, size_t len) {
#ifdef DEBUG_OUTPUT
  buffer_puts(buffer_2, "Leaving [");
  buffer_putlong(buffer_2, strarray_size(&dirstack));
  buffer_puts(buffer_2, "] '");
  buffer_put(buffer_2, x, len);
  buffer_puts(buffer_2, "'");
  buffer_putnlflush(buffer_2);
#endif

  if(strarray_size(&dirstack)) {
    stralloc_free(&dirs.build.sa);

    dirs.build.sa.s = strarray_pop(&dirstack);
    dirs.build.sa.len = dirs.build.sa.s ? str_len(dirs.build.sa.s) : 0;
  }
}

void
input_process_path_b(const char* y, size_t len, stralloc* out) {
#ifdef DEBUG_OUTPUT_
  buffer_puts(buffer_2, "input_process_path_b \"");
  buffer_put(buffer_2, y, len);
  buffer_puts(buffer_2, "\"");
  buffer_putnlflush(buffer_2);
#endif

  stralloc_init(out);
  path_append(dirs.build.sa.s, dirs.build.sa.len, out);
  path_append(y, len, out);
  stralloc_nul(out);
  path_relative_to(out->s, dirs.out.sa.s, out);
  path_prepends(".", out);
  stralloc_nul(out);

  path_collapse_sa(out);
  stralloc_nul(out);
#ifdef DEBUG_OUTPUT_
  buffer_puts(buffer_2, "out \"");
  buffer_puts(buffer_2, out->s);
  buffer_puts(buffer_2, "\"");
  buffer_putnlflush(buffer_2);
#endif
}

void
input_process_path(const char* y, stralloc* out) {
  input_process_path_b(y, str_len(y), out);
}

int
input_process_command(stralloc* cmd, int argc, char* argv[], const char* file, size_t line) {
  size_t n, len;
  bool do_rule;
  int i, compile = 0, link = 0, lib = 0, objects = 0;
  const char *x, *y;
  char **p, **end = argv + argc;
  stralloc output, dir, path;
  strlist args, files, flags, libs;
  stralloc reldir = {0, 0, 0};

  path_relative_to_b(dirs.out.sa.s, dirs.out.sa.len, dirs.build.sa.s, dirs.build.sa.len, &reldir);

  stralloc_init(&output);
  stralloc_init(&dir);
  strlist_init(&args, ' ');
  strlist_init(&files, ' ');
  strlist_init(&flags, ' ');
  strlist_init(&libs, ' ');

  for(i = 0; i < argc; i++) {
    if(is_source(argv[i])) {
      compile = 1;
      break;
    }
  }

  if(compile || link || lib) {

  } else if(!is_command_b(cmd->s, cmd->len))
    return 0;

  strlist_push_sa(&args, cmd);

  if(stralloc_ends(cmd, "make")) {
    for(p = argv; (len = *p ? str_len(*p) : 0, x = *p, p < end); p++) {
      len = str_len(x);

      if(len >= 2 && !byte_diff(x, 2, "-f")) {
        if(len > 2) {
          x += 2;
          len -= 2;
        } else {
          x = *++p;
          len = str_len(x);
        }

        if(len) {
          strlist_pushb(&files, x, len);
          strlist_push(&flags, "-f");
          strlist_pushb(&flags, x, len);
        }

        continue;
      }

      if(is_var(x))
        continue;

      if(output.len)
        stralloc_catc(&output, ' ');

      stralloc_cats(&output, x);
      strlist_push(&flags, x);
    }
  } else {
    for(p = argv; (len = *p ? str_len(*p) : 0, x = *p, p < end); p++) {
      int src = is_source_b(x, len);

      if(byte_equal(x, 2, "-c") || src) {
        compile = 1;
        link = 0;
      }

      if(src)
        sources_add_b(x, byte_chrs(x, len, " \t\n\v\r", 5));
    }
    for(p = argv; (len = *p ? str_len(*p) : 0, x = *p, p < end); p++) {
      len = str_len(x);

      if(len >= 2 && !byte_diff(x, 2, "-o")) {
        if(len > 2) {
          x += 2;
          len -= 2;
        } else {
          x = *++p;
          len = str_len(x);
        }

        if(len) {
          strlist_replaceb(&args, x, len, "$@", 2);
          stralloc_copyb(&output, x, str_chrs(x, "\r\n", 2));
        }
      }
    }
    for(p = argv; (len = *p ? str_len(*p) : 0, x = *p, p < end); p++) {
      len = str_len(x);

      if(len >= 2 && x[0] == '>') {
        y = x + 1 + (x[1] == '>');
        stralloc_copyb(&output, y, str_chrs(y, "\r\n", 2));
      } else if(len >= 2 && byte_equal(x, 2, "-o")) {
        y = (x[2] == '\0') ? *++p : x + 2;
        input_process_path(y, &path);
        strlist_push(&args, "-o");
        strlist_push(&args, path.s);
        strlist_push(&flags, "-o");
        strlist_push(&flags, path.s);
        stralloc_copyb(&output, y, str_chrs(y, "\r\n", 2));
        stralloc_free(&path);
        continue;
      } else if(len >= 2 && byte_equal(x, 2, "-M")) {
        switch(x[2]) {
          case 'D':
          case 'P':
          case 'M': break;
          case 'T':
          case 'F': {
            y = x + ((x[2] == '\0') ? 3 : 2);
            ++p;
            break;
          }
        }
        continue;
      } else if(len >= 2 && byte_equal(x, 2, "-D")) {
        y = x + ((x[2] == '\0') ? 3 : 2);
        push_define(y);
      } else if(len >= 2 && byte_equal(x, 2, "-I")) {
        size_t i = (x[2] == '\0') ? 3 : 2;
        size_t n = len - i;
        y = x + i;
        input_process_path(y, &path);
        strlist_push(&args, "-I");
        stralloc_cats(&args.sa, path.s);
        strlist_push(&flags, "-I");
        stralloc_cats(&flags.sa, path.s);

        if(n >= 2 && y[n - 1] == '.' && y[n - 2] == '/')
          n -= 2;
        includes_add_b(y, n);
        x = y;
        stralloc_free(&path);
        continue;
      } else if(len >= 2 && byte_equal(x, 2, "-L")) {
        y = x + ((x[2] == '\0') ? 3 : 2);
        input_process_path(y, &path);
        strlist_push(&args, "-L");
        stralloc_cats(&args.sa, path.s);
        strlist_push(&flags, "-L");
        stralloc_cats(&flags.sa, path.s);
        libdirs_add(y);
        x = y;
        stralloc_free(&path);
        continue;
      } else if(len >= 2 && byte_equal(x, 2, "-l")) {
        y = x + ((x[2] == '\0') ? 3 : 2);
        strlist_push(&libs, x);
      } else if(len >= 3 && byte_equal(x, 3, "-MF")) {
        size_t i = len > 3 ? 3 : 4;
        y = x + i;
        len -= i;
      } else if(len >= 2 && byte_equal(x, 2, "-c")) {
        compile = true;
      } else if(len >= 7 && byte_equal(x, 7, "--chip=")) {
        stralloc_copys(&cfg.chip, &x[7]);
      } else if(len == 15 && byte_equal(x, 15, "--memorysummary")) {
        y = x + 16;
        var_set("MEMORYSUMMARY", y);
        strlist_push(&flags, "$(MEMORYSUMMARY)");
        ++p;
      } else if(len >= 1 && x[0] != '-') {
        y = x;
#ifdef DEBUG_OUTPUT_
        buffer_puts(buffer_2, "File \"");
        buffer_put(buffer_2, y, len);
        buffer_puts(buffer_2, "");
        buffer_putnlflush(buffer_2);
#endif
        if(is_filename_b(y, len)) {
          input_process_path_b(y, len, &path);

          strlist_push_sa(&files, &path);
          strlist_push_sa(&args, &path);
#ifdef DEBUG_OUTPUT_
          buffer_puts(buffer_2, "File '");
          buffer_put(buffer_2, path.s, path.len);
          buffer_puts(buffer_2, "'");
          buffer_putnlflush(buffer_2);
#endif
          if(is_source_sa(&path))
            sources_add_b(path.s, path.len);
          stralloc_free(&path);
        }
        x = y;
        continue;
      }

      strlist_push(&args, x);
      strlist_push(&flags, x);
      x = y;
    }
  }

  if(output.len) {
    strlist_removeb(&files, output.s, output.len);
    path_canonical_sa(&output);
    strlist_removeb(&files, output.s, output.len);
  }

  stralloc_copy(cmd, &args.sa);
  stralloc_nul(cmd);

#ifdef DEBUG_OUTPUT_
  buffer_puts(buffer_2, "\treldir = ");
  buffer_putsa(buffer_2, &reldir);
  buffer_puts(buffer_2, "\n\tbuilddir = ");
  buffer_putsa(buffer_2, &dirs.build.sa);
  buffer_puts(buffer_2, "\n\toutdir = ");
  buffer_putsa(buffer_2, &dirs.out.sa);
  buffer_puts(buffer_2, "\n\tcmd = ");
  buffer_putsa(buffer_2, cmd);
  buffer_puts(buffer_2, "\n\targs =\n\t\t");
  buffer_putsl(buffer_2, &args, "\n\t\t");
  buffer_puts(buffer_2, "\n\tfiles =\n\t\t");
  buffer_putsl(buffer_2, &files, "\n\t\t");
  buffer_putnlflush(buffer_2);
#endif

  for(p = argv; (len = *p ? str_len(*p) : 0, x = *p); p++) {
    len = str_len(x);

    if(byte_finds(x, len, exts.obj) < len || byte_finds(x, len, exts.lib) < len) {
      objects++;

      if(!compile && !lib) {
        if(objects > 1) {
          compile = 0;
          link = 1;
        }
      }
    }
  }

  if(output.len == 0) {
    stralloc sa;
    stralloc_init(&sa);
    // path_normalize_b(files.sa.s, byte_chr(files.sa.s, files.sa.len, files.sep), &sa);

    stralloc_copyb(&sa, files.sa.s, byte_chr(files.sa.s, files.sa.len, files.sep));
    stralloc_nul(&sa);

    if(compile)
      path_extension(sa.s, &output, exts.obj);

    stralloc_free(&sa);
  }

  if(!output.len) {
    if((n = strlist_count_pred(&files, &is_source_b)) < strlist_count(&files)) {
      stralloc sa;

      stralloc_init(&sa);

      if(strlist_copyat(&files, n, &output)) {
        strlist_removeb(&files, output.s, output.len);
        stralloc_nul(&output);
      }

      stralloc_free(&sa);
    }
  }

  strlist_removeb(&files, output.s, output.len);

  if(stralloc_starts(&output, "./"))
    stralloc_remove(&output, 0, 2);

  stralloc_nul(&output);

  /* if(link && stralloc_ends(&output, exts.lib)) {
     link = 0;
     lib = 1;
   }*/

#ifdef DEBUG_OUTPUT_
  buffer_puts(buffer_2, file);
  buffer_puts(buffer_2, ":");
  buffer_putulong(buffer_2, line);
  buffer_puts(buffer_2, ",\n\tobjects = ");
  buffer_putulong(buffer_2, objects);
  buffer_puts(buffer_2, ",\n\toutput = ");
  buffer_putsa(buffer_2, &output);
  buffer_puts(buffer_2, ",\n\tcmd = ");
  buffer_putsa(buffer_2, cmd);
  buffer_puts(buffer_2, ",\n\tfiles =\n\t\t");
  buffer_putsl(buffer_2, &files, ",\n\t\t");
  buffer_putnlflush(buffer_2);
#endif

  if(output.len) {
    uint32* count_ptr;

    stralloc_nul(&output);
    stralloc_zero(&dir);
    path_dirname(output.s, &dir);

    if(!(count_ptr = MAP_GET(targetdirs, dir.s, dir.len + 1))) {
      uint32 count = 0;
      MAP_INSERT(targetdirs, dir.s, dir.len + 1, &count, sizeof(count));
      count_ptr = MAP_GET(targetdirs, dir.s, dir.len + 1);
    }

    (*count_ptr)++;
  }

  n = strlist_count(&files);

  if(output.len) {
    stralloc source;
    stralloc_init(&source);
  }

  stralloc_nul(&output);

  n = strlist_count_pred(&files, &is_source_b);

  if(n > 0)
    compile = 1;

  do_rule = (n || strlist_count_pred(&files, &is_object_b)) || output.len;

  if(stralloc_starts(&output, "@") || stralloc_starts(&output, "/tmp"))
    do_rule = false;

  if(do_rule && (lib || link || compile)) {

#ifdef DEBUG_OUTPUT
    buffer_puts(buffer_2, "Create ");
    buffer_puts(buffer_2, lib ? "lib" : link ? "link" : compile ? "compile" : "other");
    buffer_puts(buffer_2, " RULE\n\toutput = ");
    buffer_putsa(buffer_2, &output);

    buffer_puts(buffer_2, "\n\tcommand = ");
    buffer_put(buffer_2, cmd->s, scan_nonwhitenskip(cmd->s, cmd->len));
    buffer_puts(buffer_2, "\n\tfiles =\n\t\t");
    buffer_putsl(buffer_2, &files, ",\n\t\t");
    buffer_putnlflush(buffer_2);
#endif

    stralloc_catc(cmd, ' ');
    stralloc_cat(cmd, &flags.sa);
    stralloc_nul(cmd);

    {
      stralloc* sacmd;
      size_t pathlen;
      target* rule;

      sacmd = &args.sa;

      /*if(compile) {
        sacmd = &commands.compile;
      } else if(link) {
        sacmd = &commands.link;
      } else if(lib) {
        sacmd = &commands.lib;
      }*/

      pathlen = reldir.len;
      stralloc_cat(&reldir, &output);

      if((rule = generate_single_rule(&output, sacmd))) {
        rule->type = compile ? COMPILE : lib ? LIB : link ? LINK : 0;

        strlist_foreach(&files, x, n) {
          if(compile && byte_ends(x, n, exts.obj))
            continue;

          if(n >= 2 && byte_equal(x, 2, "./")) {
            x += 2;
            n -= 2;
          }

          set_add(&rule->prereq, x, n);
        }

        strlist_foreach(&libs, x, n) {
          target* dep;

          if((dep = rule_find_lib(x, n, exts.lib, exts.slib))) {
            set_adds(&rule->prereq, dep->name);
            rule_add_dep(rule, dep);
          } else {
            if(n >= 2 && byte_equal(x, 2, "-l")) {
              x += 2;
              n -= 2;
            }

            set_add(&link_libraries, x, n);
          }
        }

#ifdef DEBUG_OUTPUT_
        buffer_puts(buffer_2, "rule = '");
        buffer_puts(buffer_2, rule->name);
        buffer_puts(buffer_2, "', prereq = ");
        buffer_putset(buffer_2, &rule->prereq, " ", 1);
        buffer_putnlflush(buffer_2);
#endif
      }

      /*   if(compile) {
           stralloc_copy(&rule->recipe, &commands.compile);
         } else if(link) {
           stralloc_copy(&rule->recipe, &commands.link);
         } else if(lib) {
           stralloc_copy(&rule->recipe, &commands.lib);
         }*/

      if(link) {
        target* all = rule_get("all");

        set_adds(&all->prereq, rule->name);
      }

#ifdef DEBUG_OUTPUT
      rule_dump(rule);
#endif
    }
  }

  stralloc_free(&output);
  strlist_free(&files);

  return link ? 2 : compile ? 1 : 0;
}

int
input_process_line(const char* x, size_t n, const char* file, size_t line) {
  size_t idx = 0;
  int ret = 0;
  char** av;
  stralloc command;
  strarray args;

#ifdef DEBUG_OUTPUT_
  buffer_puts(buffer_2, "input_process_line \"");
  buffer_put_escaped(buffer_2, x, n, fmt_escapecharshell);
  buffer_puts(buffer_2, "\"");
  buffer_putnlflush(buffer_2);
#endif

  stralloc_init(&command);
  strarray_init(&args);

  while(n > 0) {
    size_t i;

    if((i = scan_whitenskip_escaped(x, n)) == n)
      break;

    x += i;
    n -= i;

    if(n >= 7 && *x == '[') {
      if(n > 16 && !byte_diff(&x[7], 9, "Building "))
        i = 16;
      else if((i = byte_chr(x, n, ']')) < n)
        i++;

      x += i;
      n -= i;

      continue;
    }

    i = scan_nonwhitenskip_escaped(x, n);

    if(idx == 0) {
      if(i >= 8 && byte_equal(x, 8, "depbase=")) {

        if((i = byte_chr(x, n, ' ')) < n)
          i++;

        x += i;
        n -= i;
        i = byte_chr(x, n, ' ');
        strarray_pushb(&args, x, i);
        x += i;
        n -= i;

        if((i = byte_chr(x, n, '\n')) < n)
          i++;

        x += i;
        n -= i;

        continue;
      }

      if(i >= 2 && x[0] == '"' && x[i - 1] == '"')
        stralloc_copyb(&command, x + 1, i - 2);
      else
        stralloc_copyb(&command, x, i);

    } else if(byte_chrs(x, i, "*?[", 3) < i) {
      bool same_dir;
      stralloc cwd, fullpath;

      stralloc_init(&cwd);
      stralloc_init(&fullpath);
      path_getcwd(&cwd);
      stralloc_nul(&cwd);
      strlist_nul(&dirs.build);

      same_dir = stralloc_equal(&cwd, &dirs.build.sa);

      if(!same_dir) {
        errno = 0;

        if(chdir(dirs.build.sa.s) == -1)
          errmsg_warnsys("chdir(): ", dirs.build.sa.s, 0);
      }

      stralloc_copy(&fullpath, &dirs.build.sa);
      stralloc_catc(&fullpath, PATHSEP_C);
      stralloc_catb(&fullpath, x, i);
      stralloc_nul(&fullpath);

      strarray_glob(&args, fullpath.s);

#ifdef DEBUG_OUTPUT
      if(strarray_size(&args) >= 1) {
        buffer_puts(buffer_2, "glob = ");
        strarray_dump(buffer_2, &args);
        buffer_putnlflush(buffer_2);
      }
#endif

      if(!same_dir)
        (void)chdir(cwd.s);

      stralloc_free(&cwd);
      stralloc_free(&fullpath);

    } else if(i == 2 && byte_equal(x, 2, "&&")) {
      break;
    } else {
      strarray_pushb(&args, x, i);
    }

    x += i;
    n -= i;
    idx++;
  }

  if(strarray_size(&args) && command.s) {
    if(*(av = strarray_to_argv(&args))) {
      int ac = strarray_size(&args);

      ret = input_process_command(&command, ac, av, file, line);
    }

    if(av)
      alloc_free(av);
  }

  stralloc_free(&command);
  strarray_free(&args);

  return ret;
}

void
input_process_rules(target* all, char psa) {
  MAP_PAIR_T t;
  strlist args, builddir, outdir;

  strlist_init(&args, '\0');
  strlist_init(&builddir, PATHSEP_C);
  strlist_init(&outdir, PATHSEP_C);

  stralloc_zero(&commands.compile);

  // hashmap_free(&(vars));
  MAP_DESTROY(vars);
  MAP_NEW(vars);

  MAP_FOREACH(rules, t) {
    const char* name = MAP_ITER_KEY(t);
    target* rule = MAP_ITER_VALUE(t);
    size_t dlen;
    char* dep = set_at_n(&rule->prereq, -1, &dlen);
    bool link = dep ? is_object_b(dep, dlen) : 0;
    bool compile = !link && is_object(name);

    if(compile || link) {
      strlist cmds;

      strlist_init(&cmds, '\0');
      strlist_fromq(&cmds, rule->recipe.s, rule->recipe.len, " \t\r\n", "\"'`");

      if(strlist_count(&args) == 0)
        strlist_copy(&args, &cmds);
      else
        strlist_intersection(&args, &cmds, &args);

      strlist_free(&cmds);
    }

    if(compile) {
      strlist path;

      strlist_init(&path, '\0');
      strlist_froms(&path, rule->name, PATHSEP_C);

      if(strlist_count(&builddir) == 0)
        strlist_copy(&builddir, &path);
      else
        strlist_intersection(&builddir, &path, compile ? &builddir : &outdir);

      strlist_free(&path);
    }

    if(link)
      path_dirname(rule->name, &outdir.sa);

#ifdef DEBUG_OUTPUT_
    buffer_putm_internal(buffer_2, "Rule: ", name, link ? " (link)" : compile ? " (compile)" : 0, NULL);
    buffer_putnlflush(buffer_2);
#endif
  }

#ifdef DEBUG_OUTPUT
  buffer_puts(buffer_2, "args: ");
  buffer_putsl(buffer_2, &args, " ");
  buffer_putnlflush(buffer_2);
  buffer_puts(buffer_2, "builddir: ");
  buffer_putsl(buffer_2, &builddir, "/");
  buffer_putnlflush(buffer_2);
#endif

  stralloc_copy(&dirs.work.sa, &builddir.sa);
  if(!stralloc_endc(&dirs.work.sa, PATHSEP_C))
    stralloc_catc(&dirs.work.sa, PATHSEP_C);
  strlist_nul(&dirs.work);

#ifdef DEBUG_OUTPUT_
  buffer_puts(buffer_2, "outdir:\n\t");
  buffer_putsa(buffer_2, &outdir.sa);
  buffer_putnlflush(buffer_2);
#endif

  if(!var_isset(srcdir_varname)) {
    path_relative_b(dirs.out.sa.s, dirs.out.sa.len, &dirs.out.sa);

    var_setb(srcdir_varname, dirs.out.sa.s, dirs.out.sa.len);
  }

  {
    size_t count;
    ssize_t found;
    var_t *cflags = var_list("CFLAGS", psa), *cc = var_list("CC", psa), *defs = var_list("DEFS", psa), *common = var_list("COMMON_FLAGS", psa);

    stralloc_zero(&defs->value.sa);

#ifdef DEBUG_OUTPUT
    buffer_puts(buffer_2, "CFLAGS before: ");
    buffer_putsl(buffer_2, &cflags->value, " ");
    buffer_putnlflush(buffer_2);
#endif

    strlist_free(&cflags->value);
    strlist_init(&cflags->value, '\0');
    count = strlist_count(&args);
    strlist_slice(&cflags->value, &args, 1, count);
    strlist_at_sa(&args, &cc->value.sa, 0);
    buffer_puts(buffer_2, "CC before: ");
    buffer_putsl(buffer_2, &cc->value, " ");
    buffer_putnlflush(buffer_2);

    if((found = strlist_match(&args, "--chip=*", 0)) >= 0) {
      char* chip = strlist_at(&args, found);

      chip += str_chr(chip, '=') + 1;
      stralloc_copys(&cfg.chip, chip);
      stralloc_nul(&cfg.chip);
      stralloc_replaces(&cflags->value.sa, cfg.chip.s, "$(CHIP)");
      stralloc_replaces(&cflags->value.sa, cfg.chip.s, "$(CHIP)");
      // stralloc_lower(&cfg.chip);

#ifdef DEBUG_OUTPUT
      buffer_puts(buffer_2, "Chip: ");
      buffer_putsa(buffer_2, &cfg.chip);
      buffer_putnlflush(buffer_2);
#endif
    }
    strlist_filter(&cflags->value, &defs->value, &cflags->value, "-D*");

    common->value.sep = '\0';
    strlist_filter(&cflags->value, &common->value, &cflags->value, "--*format=*");

    if(common->value.sa.len)
      strlist_push(&cflags->value, "$(COMMON_FLAGS)");

    strlist_nul(&cc->value);

    {
      strlist compiler;
      char* s;
      size_t n;

      strlist_init(&compiler, '/');
      strlist_froms(&compiler, cc->value.sa.s + 1, PATHSEP_C);

      strlist_foreach(&compiler, s, n) {
        if(is_version_b(s, n)) {
          var_setb("VER", s, n);
          stralloc_replace(&cc->value.sa, s - compiler.sa.s + 1, n, "$(VER)", 6);
          break;
        }
      }
    }

#ifdef DEBUG_OUTPUT
    buffer_puts(buffer_2, "CC after: ");
    buffer_putsa(buffer_2, &cc->value.sa);
    buffer_putnlflush(buffer_2);
    buffer_puts(buffer_2, "CFLAGS after: ");
    buffer_putsl(buffer_2, &cflags->value, " ");
    buffer_putnlflush(buffer_2);
#endif
  }
  /*MAP_FOREACH(rules, t) {
      target* rule = MAP_ITER_VALUE(t);
      stralloc* sa = &commands.v[rule->type];
      if(sa->s && sa->len)
        stralloc_copy(&rule->recipe, sa);
    }*/

  MAP_FOREACH(rules, t) {
    const char* name = MAP_ITER_KEY(t);
    target* rule = MAP_ITER_VALUE(t);
    size_t dlen;
    char* dep = set_at_n(&rule->prereq, 0, &dlen);
    bool link = dep ? is_object_b(dep, dlen) : false;
    bool compile = !link && is_object(name);

    if(compile || link) {
      char* x;
      size_t i, n;
      set_iterator_t it;
      stralloc cmd;

      stralloc_init(&cmd);
      stralloc_copy(&cmd, &rule->recipe);

      /*if(link && (i = stralloc_findb(&cmd, "\n", 1)) < cmd.len)
        stralloc_trunc(&cmd, i);*/

      stralloc_0(&cmd);

      strlist_foreach(&args, x, n) { REMOVE(&cmd, i, x, n); }

      set_foreach(&rule->prereq, it, x, n) { REMOVE(&cmd, i, x, n); }

      stralloc_trimr(&cmd, "\0\r\n", 3);
      stralloc_nul(&cmd);

#ifdef DEBUG_OUTPUT_
      buffer_puts(buffer_2, "Prereq: ");
      buffer_putset(buffer_2, &rule->prereq, " ", 1);
      buffer_putnlflush(buffer_2);
#endif

      stralloc_prepends(&cmd, "$(CC) $(CFLAGS) ");

      if(cmd.len && cmd.s[cmd.len - 1] != ' ' && cmd.s[cmd.len - 1] != PATHSEP_C)
        stralloc_catc(&cmd, ' ');

      stralloc_cats(&cmd, set_size(&rule->prereq) > 1 ? "$^" : "$<");

      if((i = stralloc_finds(&cmd, name)) < cmd.len)
        stralloc_replace(&cmd, i, str_len(name), "$@", 2);

      if(compile) {
        if(commands.compile.len == 0 /* && !infile*/)
          stralloc_copy(&commands.compile, &cmd);

        /*    stralloc_copy(&rule->recipe, &commands.compile); */
        stralloc_nul(&rule->recipe);
      }

      stralloc_free(&cmd);
    }

#ifdef DEBUG_OUTPUT_
    buffer_putm_internal(buffer_2, "Rule: ", name, compile ? " (compile)" : link ? " (link)" : "", "\n\t", NULL);
    buffer_putsa(buffer_2, &rule->recipe);
    buffer_putnlflush(buffer_2);
#endif
  }
}

void
input_process(const char* infile, target* all, char psa) {
  const char* x;
  size_t n, line = 1;

  path_dirname(infile, &dirs.this.sa);
  strlist_nul(&dirs.this);

  if((x = mmap_read(infile, &n))) {
    while(n > 0) {
      size_t i = scan_lineskip_escaped(x, n);
      size_t ln = byte_count(x, i, '\n');

      if(!(i > 2 && x[0] == '-' && x[1] == '-')) {
        size_t pos;

        if((pos = byte_finds(x, i, "ing directory '")) < i) {
          bool enter = byte_equal(&x[pos - 5], 5, "Enter");
          size_t len;

          pos += 15;
          len = byte_chr(&x[pos], i - 15, '\'');

          if(enter)
            builddir_enter(&x[pos], len);
          else
            builddir_leave(&x[pos], len);

        } else if(i > 0) {
          size_t len = i;

          if(len > 0 && x[len - 1] == '\n')
            --len;

          if(len > 0 && x[len - 1] == '\r')
            --len;

          input_process_line(x, len, infile, line);
        }
      }

      x += i;
      n -= i;
      line += ln;
    }

    mmap_unmap(x, n);
  }

  input_process_rules(all, psa);
}
