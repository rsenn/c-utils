#include "input.h"
#include "ansi.h"
#include "var.h"
#include "generate.h"
#include "../../genmakefile.h"
#include "../../lib/set.h"
#include <errno.h>

set_t common_flags = SET();
strlist common_flags_list = {{0}, ' '};

/**
 * @brief      Remove from stralloc
 *
 * @param      sa    stralloc
 * @param      x     Buffer
 * @param      n     length
 */
#define REMOVE(sa, x, n) \
  do { \
    size_t j = n; \
    size_t i = stralloc_findb((sa), x, n); \
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
    } \
  } while(0)

/**
 * @brief      Enter build directory
 *
 * @param[in]  x     Directory string
 * @param[in]  len   Directory length
 */
void
builddir_enter(const char* x, size_t len) {
  stralloc tmp;

  stralloc_init(&tmp);
  strarray_emplace_sa(&dirstack, &dirs.obj.sa);
  stralloc_copyb(&dirs.obj.sa, x, len);

  path_relative_b(dirs.obj.sa.s, dirs.obj.sa.len, &tmp);
  set_addsa(&build_directories, &tmp);

#ifdef DEBUG_OUTPUT_
  buffer_puts(debug_buf, "Entering [");
  buffer_putlong(debug_buf, strarray_size(&dirstack));
  buffer_puts(debug_buf, "] '");
  buffer_putsa(debug_buf, &tmp);
  buffer_puts(debug_buf, "'");
  buffer_putnlflush(debug_buf);
#endif

  stralloc_free(&tmp);
}

/**
 * @brief      Leave build directory
 *
 * @param[in]  x     Directory string
 * @param[in]  len   Directory length
 */
void
builddir_leave(const char* x, size_t len) {
#ifdef DEBUG_OUTPUT_
  buffer_puts(debug_buf, "Leaving [");
  buffer_putlong(debug_buf, strarray_size(&dirstack));
  buffer_puts(debug_buf, "] '");
  buffer_put(debug_buf, x, len);
  buffer_puts(debug_buf, "'");
  buffer_putnlflush(debug_buf);
#endif

  if(strarray_size(&dirstack)) {
    stralloc_free(&dirs.obj.sa);

    dirs.obj.sa.s = strarray_pop(&dirstack);
    dirs.obj.sa.len = dirs.obj.sa.s ? str_len(dirs.obj.sa.s) : 0;
  }
}

/**
 * @brief      Process a path
 *
 * @param[in]  y     Path buffer
 * @param[in]  len   Path length
 * @param      out   Output
 */
void
input_process_path_b(const char* y, size_t len, stralloc* out) {
#ifdef DEBUG_OUTPUT_
  buffer_putm_internal(debug_buf, BLUE256, __func__, NC, " '", 0);
  buffer_put(debug_buf, y, len);
  buffer_puts(debug_buf, "'");
  buffer_putnlflush(debug_buf);
#endif

  stralloc_init(out);

  if(byte_starts(y, len, "CMakeFiles")) {
    size_t n = path_skip(y, len);
    y += n;
    len -= n;
  }

  size_t n;

  if((n = byte_finds(y, len, ".dir/")) < len) {
    n += 5;
    y += n;
    len -= n;
  }

  if(path_is_absolute_b(y, len)) {
    stralloc tmp;

    stralloc_init(&tmp);
    stralloc_copyb(&tmp, y, len);
    stralloc_nul(&tmp);
    path_collapse_sa(&tmp);

    path_relative_to_b(tmp.s, tmp.len, dirs.out.sa.s, dirs.out.sa.len, out);
    stralloc_free(&tmp);
  } else {
    stralloc tmp;

    stralloc_init(&tmp);
    path_append(dirs.obj.sa.s, dirs.obj.sa.len, &tmp);
    path_append(y, len, &tmp);
    stralloc_nul(&tmp);
    path_relative_to(tmp.s, dirs.out.sa.s, out);
    stralloc_free(&tmp);
    path_prepends(".", out);
  }

  path_collapse_sa(out);
  stralloc_nul(out);

#ifdef DEBUG_OUTPUT_
  buffer_putm_internal(debug_buf, BLUE256, __func__, NC, " out='");
  buffer_puts(debug_buf, out->s);
  buffer_puts(debug_buf, "'");
  buffer_putnlflush(debug_buf);
#endif
}

/**
 * @brief      Process a path
 *
 * @param[in]  y     Path buffer
 * @param      out   Output
 */
void
input_process_path(const char* y, stralloc* out) {
  input_process_path_b(y, str_len(y), out);
}

/**
 * @brief      Process a command
 *
 * @param      cmd   The command
 * @param[in]  argc  Number of arguments
 * @param      argv  The arguments array
 * @param[in]  file  Filename
 * @param[in]  line  Line number
 *
 * @return     2 on link, 1 on compile, 0 otherwise
 */
int
input_process_command(stralloc* cmd, int argc, char* argv[], const char* file, size_t line) {
  size_t n, len;
  bool do_rule;
  int i, compile = 0, link = 0, lib = 0, objects = 0;
  const char *x, *y;
  char **p, **end = argv + argc;
  stralloc out, dir, path, rel;
  strlist args, files, flags, libs;

#ifdef DEBUG_OUTPUT_
  buffer_putm_internal(debug_buf, GREEN256, __func__, NC, " '", 0);
  buffer_putsa(debug_buf, cmd);
  buffer_puts(debug_buf, "'");
  buffer_putnlflush(debug_buf);
#endif

  stralloc_init(&out);
  stralloc_init(&dir);
  stralloc_init(&path);
  stralloc_init(&rel);
  path_relative_to_b(dirs.out.sa.s, dirs.out.sa.len, dirs.obj.sa.s, dirs.obj.sa.len, &rel);

  strlist_init(&args, ' ');
  strlist_init(&files, ' ');
  strlist_init(&flags, ' ');
  strlist_init(&libs, ' ');

  for(i = 0; i < argc; i++) {
    if(is_source(argv[i]) || str_equal(argv[i], "-c")) {
      compile = 1;
      break;
    }
  }

  if(compile || link || lib) {
  } else if(!is_command_b(cmd->s, cmd->len)) {
    return 0;
  }

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

      if(out.len)
        stralloc_catc(&out, ' ');

      stralloc_cats(&out, x);
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
          stralloc_copyb(&out, x, str_chrs(x, "\r\n", 2));
        }
      }
    }

    for(p = argv; (len = *p ? str_len(*p) : 0, x = *p, p < end); p++) {
      len = str_len(x);

      if(len >= 2 && x[0] == '>') {
        y = x + 1 + (x[1] == '>');
        stralloc_copyb(&out, y, str_chrs(y, "\r\n", 2));
      } else if(len >= 2 && byte_equal(x, 2, "-o")) {
        y = (x[2] == '\0') ? *++p : x + 2;
        input_process_path(y, &path);
        strlist_push(&args, "-o");
        strlist_push(&args, path.s);
        /*       strlist_push(&flags, "-o");
               strlist_push(&flags, path.s);*/
        stralloc_copyb(&out, y, str_chrs(y, "\r\n", 2));
        stralloc_zero(&path);
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
        y = (x[2] == '\0') ? *++p : x + 2;
        push_define(y);
      } else if(len >= 2 && byte_equal(x, 2, "-I")) {
        bool bare = x[2] == '\0';
        size_t n;

        y = bare ? *++p : x + 2;
        n = bare ? str_len(y) : len - 2;

        input_process_path(y, &path);
        strlist_push(&args, "-I");
        stralloc_cats(&args.sa, path.s);
        strlist_push(&flags, "-I");
        stralloc_cats(&flags.sa, path.s);

        if(n >= 2 && y[n - 1] == '.' && y[n - 2] == '/')
          n -= 2;

        includes_add_b(y, n);
        x = y;
        stralloc_zero(&path);
        continue;
      } else if(len >= 2 && byte_equal(x, 2, "-L")) {
        y = (x[2] == '\0') ? *++p : x + 2;

        input_process_path(y, &path);

        strlist_push(&args, "-L");
        stralloc_cats(&args.sa, path.s);
        strlist_push(&flags, "-L");
        stralloc_cats(&flags.sa, path.s);

        libdirs_add(y);

        x = y;
        stralloc_zero(&path);
        continue;
      } else if(len >= 2 && byte_equal(x, 2, "-l")) {
        y = (x[2] == '\0') ? *++p : x + 2;
        strlist_push(&libs, x);
      } else if(len >= 3 && byte_equal(x, 3, "-MF")) {
        bool bare = len <= 3;

        y = bare ? *++p : x + 3;
        len = bare ? str_len(y) : len - 3;
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
        buffer_putm_internal(debug_buf, GREEN256, __func__, NC, " File '", 0);
        buffer_put(debug_buf, y, len);
        buffer_puts(debug_buf, "");
        buffer_putnlflush(debug_buf);
#endif

        if(is_filename_b(y, len)) {
          stralloc tmp;

          stralloc_init(&tmp);

          input_process_path_b(y, len, &path);

          strlist_push_sa(&files, &path);
          strlist_push_sa(&args, &path);

          if(path_is_absolute_b(path.s, path.len))
            path_relative_to_sa(&path, &dirs.out.sa, &tmp);
          else
            stralloc_copy(&tmp, &path);

          stralloc_nul(&tmp);

#ifdef DEBUG_OUTPUT_
          buffer_putm_internal(debug_buf, GREEN256, __func__, NC, " File '", 0);
          buffer_put(debug_buf, tmp.s, tmp.len);
          buffer_puts(debug_buf, "'");
          buffer_putnlflush(debug_buf);
#endif

          if(is_source_sa(&tmp))
            sources_add_b(tmp.s, tmp.len);

          stralloc_free(&tmp);
          stralloc_zero(&path);
        }

        x = y;
        continue;
      }

      strlist_push(&args, x);
      strlist_push(&flags, x);
      x = y;
    }
  }

  if(compile) {
    var_set_b("CC", cmd->s, cmd->len);
  }

  {
    const char* x;
    size_t n;
    set_t fs = SET(), tmp = SET();

    strlist_foreach(&flags, x, n) { set_add(&fs, x, n); }

    if(set_size(&common_flags)) {
      set_intersection(&tmp, &fs, &common_flags);
      set_free(&common_flags);
      common_flags = tmp;
    } else {
      common_flags = fs;
    }

    strlist_zero(&common_flags_list);

    strlist_foreach(&flags, x, n) {
      if(set_has(&fs, x, n)) {
        strlist_pushb_unique(&common_flags_list, x, n);

#ifdef DEBUG_OUTPUT
        buffer_puts(debug_buf, "Common flag: ");
        buffer_put(debug_buf, x, n);
        buffer_putnlflush(debug_buf);
#endif
      }
    }
  }

  if(out.len) {
    strlist_removeb(&files, out.s, out.len);
    path_canonical_sa(&out);
    strlist_removeb(&files, out.s, out.len);
  }

  if(out.len == 0 && compile) {
    size_t flen = byte_chr(files.sa.s, files.sa.len, files.sep);
    size_t bpos = path_basepos2(files.sa.s, flen);

    stralloc_copyb(&out, files.sa.s + bpos, flen - bpos);
    stralloc_replaces(&out, exts.src, exts.obj);
    stralloc_nul(&out);
  }

  stralloc_copy(cmd, &args.sa);
  stralloc_nul(cmd);

  for(p = argv; (len = *p ? str_len(*p) : 0, x = *p); p++) {
    len = str_len(x);

    if(byte_finds(x, len, exts.obj) < len || byte_finds(x, len, exts.lib) < len) {
      objects++;

      if(!compile && !lib) {
        if(objects > 1) {
          compile = 0;
          link = 1;
          break;
        }
      }
    }
  }

  /* Relocate output file */

  if(compile || link) {
    stralloc tmp;

    stralloc_init(&tmp);

    if(path_is_absolute_sa(&out)) {
      path_relative_to_sa(&out, &dirs.out.sa, &tmp);
      stralloc_copy(&out, &tmp);
      stralloc_nul(&out);
    }

    input_process_path_b(out.s, out.len, &tmp);

    strlist_replaceb(&args, out.s, out.len, tmp.s, tmp.len);

    stralloc_copy(&out, &tmp);
    stralloc_nul(&out);

    stralloc_free(&tmp);
  }

  /* if(!compile && !link && !lib)
     link = 1;
 */
#ifdef DEBUG_OUTPUT_
  buffer_putm_internal(debug_buf, GREEN256, __func__, NC, " '", 0);
  buffer_putsa(debug_buf, cmd);
  buffer_puts(debug_buf, "'\treldir = ");
  buffer_putsa(debug_buf, &rel);
  buffer_puts(debug_buf, "\n\tbuilddir = ");
  buffer_putsa(debug_buf, &dirs.obj.sa);
  buffer_puts(debug_buf, "\n\toutdir = ");
  buffer_putsa(debug_buf, &dirs.out.sa);
  buffer_puts(debug_buf, "\n\tcmd = ");
  buffer_putsa(debug_buf, cmd);
  buffer_puts(debug_buf, "\n\tcompile = ");
  buffer_putlong(debug_buf, compile);
  buffer_puts(debug_buf, "\n\tlink = ");
  buffer_putlong(debug_buf, link);
  buffer_puts(debug_buf, "\n\tlib = ");
  buffer_putlong(debug_buf, lib);
  buffer_puts(debug_buf, "\n\targs =\n\t\t");
  buffer_putsl(debug_buf, &args, "\n\t\t");
  buffer_puts(debug_buf, "\n\tfiles =\n\t\t");
  buffer_putsl(debug_buf, &files, "\n\t\t");
  buffer_putnlflush(debug_buf);
#endif

  if(out.len == 0) {
    stralloc tmp;

    stralloc_init(&tmp);
    // path_normalize_sa_b(files.tmp.s, byte_chr(files.tmp.s, files.tmp.len, files.sep), &tmp);

    stralloc_copyb(&tmp, files.sa.s, byte_chr(files.sa.s, files.sa.len, files.sep));
    stralloc_nul(&tmp);

    if(compile)
      path_extension(tmp.s, &out, exts.obj);

    stralloc_free(&tmp);
  }

  if(!out.len) {
    if((n = strlist_count_pred(&files, &is_source_b)) < strlist_count(&files)) {
      stralloc tmp;

      stralloc_init(&tmp);

      if(strlist_copyat(&files, n, &out)) {
        strlist_removeb(&files, out.s, out.len);
        stralloc_nul(&out);
      }

      stralloc_free(&tmp);
    }
  }

  strlist_removeb(&files, out.s, out.len);

  if(stralloc_starts(&out, "./"))
    stralloc_remove(&out, 0, 2);

  stralloc_nul(&out);

  /* if(link && stralloc_ends(&out, exts.lib)) {
     link = 0;
     lib = 1;
   }*/

#ifdef DEBUG_OUTPUT_
  buffer_puts(debug_buf, file);
  buffer_puts(debug_buf, ":");
  buffer_putulong(debug_buf, line);
  buffer_puts(debug_buf, ",\n\tobjects = ");
  buffer_putulong(debug_buf, objects);
  buffer_puts(debug_buf, ",\n\toutput = ");
  buffer_putsa(debug_buf, &out);
  buffer_puts(debug_buf, ",\n\tcmd = ");
  buffer_putsa(debug_buf, cmd);
  buffer_puts(debug_buf, ",\n\tfiles =\n\t\t");
  buffer_putsl(debug_buf, &files, ",\n\t\t");
  buffer_putnlflush(debug_buf);
#endif

  if(out.len) {
    uint32* count_ptr;

    stralloc_nul(&out);
    stralloc_zero(&dir);
    path_dirname(out.s, &dir);

    if(!(count_ptr = MAP_GET(targetdirs, dir.s, dir.len + 1))) {
      uint32 count = 0;

      MAP_INSERT2(targetdirs, dir.s, dir.len + 1, &count, sizeof(count));
      count_ptr = MAP_GET(targetdirs, dir.s, dir.len + 1);
    }

    ++(*count_ptr);
  }

  n = strlist_count(&files);

  /*if(out.len) {
    stralloc source;
    stralloc_init(&source);
  }*/

  stralloc_nul(&out);

  if((n = strlist_count_pred(&files, &is_source_b)) > 0)
    compile = 1;

  do_rule = (n || strlist_count_pred(&files, &is_object_b)) || out.len;

  if(stralloc_starts(&out, "@") || stralloc_starts(&out, "/tmp"))

    do_rule = false;

  if(do_rule && (lib || link || compile)) {
#ifdef DEBUG_OUTPUT_
    buffer_puts(debug_buf, "Create ");
    buffer_puts(debug_buf, lib ? "lib" : link ? "link" : compile ? "compile" : "other");
    buffer_puts(debug_buf, " RULE\n\toutput = ");
    buffer_putsa(debug_buf, &out);

    buffer_puts(debug_buf, "\n\tcommand = ");
    buffer_put(debug_buf, cmd->s, scan_nonwhitenskip(cmd->s, cmd->len));
    buffer_puts(debug_buf, "\n\tfiles =\n\t\t");
    buffer_putsl(debug_buf, &files, ",\n\t\t");
    buffer_putnlflush(debug_buf);
#endif

    stralloc_catc(cmd, ' ');
    stralloc_cat(cmd, &flags.sa);
    stralloc_nul(cmd);

    {
      size_t pathlen;
      target* rule;
      stralloc* sacmd = &args.sa;

      /*if(compile) {
        sacmd = &commands.compile;
      } else if(link) {
        sacmd = &commands.link;
      } else if(lib) {
        sacmd = &commands.lib;
      }*/

      pathlen = rel.len;
      stralloc_cat(&rel, &out);

      if((rule = generate_single_rule(&out, sacmd))) {
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
        buffer_puts(debug_buf, "\trule = '");
        buffer_puts(debug_buf, rule->name);
        buffer_puts(debug_buf, "'\n\tprereq = ");
        buffer_putset(debug_buf, &rule->prereq, " ", 1);
        buffer_putnlflush(debug_buf);
#endif

        /*if(compile) {
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

#ifdef DEBUG_OUTPUT_
        rule_dump(rule);
#endif
      }
    }
  }

#ifdef DEBUG_OUTPUT_
  buffer_putm_internal(debug_buf, "end ", GREEN256, __func__, NC, 0);
  buffer_putnlflush(debug_buf);
#endif

  stralloc_free(&rel);
  stralloc_free(&path);
  stralloc_free(&dir);
  stralloc_free(&out);
  strlist_free(&libs);
  strlist_free(&flags);
  strlist_free(&files);
  strlist_free(&args);

  return link ? 2 : compile ? 1 : 0;
}

/**
 * @brief      Process an input line
 *
 * @param[in]  x     Character data
 * @param[in]  n     Length of data
 * @param[in]  file  Filename
 * @param[in]  line  Line number
 *
 * @return     >= 0 on success, -1 on error
 */
int
input_process_line(const char* x, size_t n, const char* file, size_t line) {
  size_t idx = 0;
  int ret = -1;
  char** av;
  stralloc command;
  strarray args;

#ifdef DEBUG_OUTPUT_
  buffer_putm_internal(debug_buf, PINK256, __func__, NC, " \"", 0);
  buffer_put_escaped(debug_buf, x, n, fmt_escapecharshell);
  buffer_puts(debug_buf, "\"");
  buffer_putnlflush(debug_buf);
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
      strlist_nul(&dirs.obj);

      same_dir = stralloc_equal(&cwd, &dirs.obj.sa);

      if(!same_dir) {
        errno = 0;

        if(chdir(dirs.obj.sa.s) == -1)
          errmsg_warnsys("chdir(): ", dirs.obj.sa.s, 0);
      }

      stralloc_copy(&fullpath, &dirs.obj.sa);
      stralloc_catc(&fullpath, PATHSEP_C);
      stralloc_catb(&fullpath, x, i);
      stralloc_nul(&fullpath);

      strarray_glob(&args, fullpath.s);

#ifdef DEBUG_OUTPUT_
      if(strarray_size(&args) >= 1) {
        buffer_puts(debug_buf, "glob = ");
        strarray_dump(debug_buf, &args);
        buffer_putnlflush(debug_buf);
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

#ifdef DEBUG_OUTPUT_
  buffer_putm_internal(debug_buf, "end ", PINK256, __func__, NC, 0);
  buffer_putnlflush(debug_buf);
#endif

  stralloc_free(&command);
  strarray_free(&args);

  return ret;
}

/**
 * @brief      Process all rule_map
 *
 * @param      all   "all" rule
 */
void
input_process_rules(target* all) {
  MAP_PAIR_T t;
  strlist args, builddir, outdir;
  var_t *cflags, *cc, *defs, *includes, *libs, *common;
  size_t count;
  ssize_t found;

  strlist_init(&args, '\0');
  strlist_init(&builddir, PATHSEP_C);
  strlist_init(&outdir, PATHSEP_C);

  stralloc_zero(&commands.compile);

#ifdef DEBUG_OUTPUT
  buffer_putm_internal(debug_buf, YELLOW256, __func__, NC, " Rules count: ", 0);
  buffer_putlong(debug_buf, MAP_SIZE(rule_map));
  buffer_putnlflush(debug_buf);
#endif

  /* MAP_DESTROY(vars);
   MAP_NEW(vars);*/

  MAP_FOREACH(rule_map, t) {
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
      strlist sl;

      strlist_init(&sl, '\0');
      strlist_froms(&sl, rule->name, PATHSEP_C);

      if(strlist_count(&builddir) == 0)
        strlist_copy(&builddir, &sl);
      else
        strlist_intersection(&builddir, &sl, compile ? &builddir : &outdir);

      strlist_free(&sl);
    }

    if(link)
      path_dirname(rule->name, &outdir.sa);

#ifdef DEBUG_OUTPUT
    buffer_putm_internal(debug_buf,
                         YELLOW256,
                         __func__,
                         NC,
                         " Rule: ",
                         name,
                         link      ? " (link)"
                         : compile ? " (compile)"
                                   : 0,
                         NULL);
    buffer_putnlflush(debug_buf);
#endif
  }

#ifdef DEBUG_OUTPUT_
  buffer_puts(debug_buf, "args: ");
  buffer_putsl(debug_buf, &args, " ");
  buffer_putnlflush(debug_buf);
  buffer_puts(debug_buf, "builddir: ");
  buffer_putsl(debug_buf, &builddir, "/");
  buffer_putnlflush(debug_buf);
#endif

  if(builddir.sa.s) {
    stralloc_copy(&dirs.work.sa, &builddir.sa);

    if(!stralloc_endc(&dirs.work.sa, PATHSEP_C))
      stralloc_catc(&dirs.work.sa, PATHSEP_C);
  }

  strlist_nul(&dirs.work);

#ifdef DEBUG_OUTPUT_
  buffer_puts(debug_buf, "outdir:\n\t");
  buffer_putsa(debug_buf, &outdir.sa);
  buffer_putnlflush(debug_buf);
#endif

  if(!var_isset(sourcedir_varname)) {
    path_relative_b(dirs.out.sa.s, dirs.out.sa.len, &dirs.out.sa);

    var_set_b(sourcedir_varname, dirs.out.sa.s, dirs.out.sa.len);
  }

  cflags = var_list("CFLAGS", ' ');
  cc = var_list("CC", ' ');
  defs = var_list("DEFS", ' ');
  includes = var_list("INCLUDES", ' ');
  libs = var_list("LIBS", ' ');
  common = var_list("COMMON_FLAGS", ' ');

  stralloc_zero(&defs->value.sa);

#ifdef DEBUG_OUTPUT
  buffer_putm_internal(debug_buf, YELLOW256, __func__, NC, " CFLAGS before: ", 0);
  buffer_putsl(debug_buf, &cflags->value, " ");
  buffer_putnlflush(debug_buf);
  buffer_putm_internal(debug_buf, YELLOW256, __func__, NC, " CC before: ", 0);
  buffer_putsl(debug_buf, &cc->value, " ");
  buffer_putnlflush(debug_buf);
  buffer_putm_internal(debug_buf, YELLOW256, __func__, NC, " DEFS before: ", 0);
  buffer_putsl(debug_buf, &defs->value, " ");
  buffer_putnlflush(debug_buf);
  buffer_putm_internal(debug_buf, YELLOW256, __func__, NC, " INCLUDES before: ", 0);
  buffer_putsl(debug_buf, &includes->value, " ");
  buffer_putnlflush(debug_buf);
  buffer_putm_internal(debug_buf, YELLOW256, __func__, NC, " LIBS before: ", 0);
  buffer_putsl(debug_buf, &libs->value, " ");
  buffer_putnlflush(debug_buf);
  buffer_putm_internal(debug_buf, YELLOW256, __func__, NC, " COMMON_FLAGS before: ", 0);
  buffer_putsl(debug_buf, &common->value, " ");
  buffer_putnlflush(debug_buf);
#endif

  /*strlist_free(&cflags->value);
  strlist_init(&cflags->value, '\0');*/

  count = strlist_count(&args);
  // strlist_slice(&cflags->value, &args, 1, count);
  // strlist_at_sa(&args, &cc->value.sa, 0);

  if((found = strlist_match(&args, "--chip=*", 0)) >= 0) {
    char* chip = strlist_at(&args, found);

    chip += str_chr(chip, '=') + 1;
    stralloc_copys(&cfg.chip, chip);
    stralloc_nul(&cfg.chip);
    stralloc_replaces(&cflags->value.sa, cfg.chip.s, "$(CHIP)");
    stralloc_replaces(&cflags->value.sa, cfg.chip.s, "$(CHIP)");
    // stralloc_lower(&cfg.chip);

#ifdef DEBUG_OUTPUT
    buffer_puts(debug_buf, "Chip: ");
    buffer_putsa(debug_buf, &cfg.chip);
    buffer_putnlflush(debug_buf);
#endif
  }

  strlist_filter(&cflags->value, &defs->value, &cflags->value, "-D*");

  common->value.sep = '\0';
  strlist_filter(&cflags->value, &common->value, &cflags->value, "--*format=*");

  strlist_nul(&cc->value);

  {
    strlist compiler;
    char* s;
    size_t n;

    strlist_init(&compiler, '/');
    strlist_froms(&compiler, cc->value.sa.s + 1, PATHSEP_C);

    strlist_foreach(&compiler, s, n) {
      if(is_version_b(s, n)) {
        var_set_b("VER", s, n);
        stralloc_replace(&cc->value.sa, s - compiler.sa.s + 1, n, "$(VER)", 6);
        break;
      }
    }

    strlist_free(&compiler);
  }

  /*MAP_FOREACH(rule_map, t) {
      target* rule = MAP_ITER_VALUE(t);
      stralloc* sa = &commands.v[rule->type];

      if(sa->s && sa->len)
        stralloc_copy(&rule->recipe, sa);
    }*/

  MAP_FOREACH(rule_map, t) {
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
      strlist cmd;

      strlist_init(&cmd, ' ');
      stralloc_copy(&cmd.sa, &rule->recipe);

      strlist_foreach(&cmd, x, n) {
        if(n > 2) {
          if(x[0] == '-') {
            switch(x[1]) {
              case 'D': {
                strlist_pushb_unique(&defs->value, x, n);
                break;
              }

              case 'I': {
                strlist_pushb_unique(&includes->value, x, n);
                break;
              }

              case 'L':
              case 'l': {
                strlist_pushb_unique(&libs->value, x, n);
                break;
              }

              case 'W': {
                if(x[2] == 'a' || x[2] == 'l')
                  break;
              }

              default: {
                if(strlist_pushb_unique(&cflags->value, x, n)) {
#ifdef DEBUG_OUTPUT
                  buffer_puts(debug_buf, "CFLAGS: ");
                  buffer_put(debug_buf, x, n);
                  buffer_putnlflush(debug_buf);
#endif
                }

                break;
              }
            }
          }
        }
      }

      /*if(link && (i = stralloc_findb(&cmd.sa, "\n", 1)) < cmd.sa.len)
        stralloc_trunc(&cmd.sa, i);*/

      stralloc_0(&cmd.sa);

      strlist_foreach(&args, x, n) { REMOVE(&cmd.sa, x, n); }

      set_foreach(&rule->prereq, it, x, n) { REMOVE(&cmd.sa, x, n); }

      stralloc_trimr(&cmd.sa, "\0\r\n", 3);
      stralloc_nul(&cmd.sa);

#ifdef DEBUG_OUTPUT_
      buffer_puts(debug_buf, "Prereq: ");
      buffer_putset(debug_buf, &rule->prereq, " ", 1);
      buffer_putnlflush(debug_buf);
#endif

      stralloc_prepends(&cmd.sa, "$(CC) $(CFLAGS) ");

      if(cmd.sa.len && cmd.sa.s[cmd.sa.len - 1] != ' ' && cmd.sa.s[cmd.sa.len - 1] != PATHSEP_C)
        stralloc_catc(&cmd.sa, ' ');

      stralloc_cats(&cmd.sa, set_size(&rule->prereq) > 1 ? "$^" : "$<");

      if((i = stralloc_finds(&cmd.sa, name)) < cmd.sa.len)
        stralloc_replace(&cmd.sa, i, str_len(name), "$@", 2);

      if(compile) {
        if(commands.compile.len == 0 /* && !infile*/)
          stralloc_copy(&commands.compile, &cmd.sa);

        /*stralloc_copy(&rule->recipe, &commands.compile); */
        stralloc_nul(&rule->recipe);
      }

      strlist_free(&cmd);
    }

#ifdef DEBUG_OUTPUT_
    buffer_putm_internal(debug_buf,
                         YELLOW256,
                         __func__,
                         NC,
                         " Rule: ",
                         name,
                         compile ? " (compile)"
                         : link  ? " (link)"
                                 : "",
                         "\n\t",
                         NULL);
    buffer_putsa(debug_buf, &rule->recipe);
    buffer_putnlflush(debug_buf);
#endif
  }

  /*  if(common->value.sa.len)
      strlist_push(&cflags->value, "$(COMMON_FLAGS)");*/

#ifdef DEBUG_OUTPUT
  buffer_putm_internal(debug_buf, YELLOW256, __func__, NC, " CFLAGS after: ", 0);
  buffer_putsl(debug_buf, &cflags->value, " ");
  buffer_putnlflush(debug_buf);
  buffer_putm_internal(debug_buf, YELLOW256, __func__, NC, " CC after: ", 0);
  buffer_putsl(debug_buf, &cc->value, " ");
  buffer_putnlflush(debug_buf);
  buffer_putm_internal(debug_buf, YELLOW256, __func__, NC, " DEFS after: ", 0);
  buffer_putsl(debug_buf, &defs->value, " ");
  buffer_putnlflush(debug_buf);
  buffer_putm_internal(debug_buf, YELLOW256, __func__, NC, " INCLUDES after: ", 0);
  buffer_putsl(debug_buf, &includes->value, " ");
  buffer_putnlflush(debug_buf);
  buffer_putm_internal(debug_buf, YELLOW256, __func__, NC, " LIBS after: ", 0);
  buffer_putsl(debug_buf, &libs->value, " ");
  buffer_putnlflush(debug_buf);
  buffer_putm_internal(debug_buf, YELLOW256, __func__, NC, " COMMON_FLAGS after: ", 0);
  buffer_putsl(debug_buf, &common->value, " ");
  buffer_putnlflush(debug_buf);
#endif

#ifdef DEBUG_OUTPUT
  buffer_putm_internal(debug_buf, "end ", YELLOW256, __func__, NC, 0);
  buffer_putnlflush(debug_buf);
#endif

  strlist_free(&args);
  strlist_free(&builddir);
  strlist_free(&outdir);
}

/* A --infile line can come decorated with arbitrary junk in front of
 * its actual argument list -- a shell "set -x" trace's "+ ", a
 * logging LD_PRELOAD shim's timestamp/pid/basename banner, or
 * anything else -- and none of that shape is known ahead of time.
 * Instead of hardcoding any particular format, input_word_is_command()
 * + input_find_command_start() locate, on a single line, the first
 * word that (a) actually resolves to a real command and (b) is
 * followed only by words that still look like plausible arguments
 * (see input_word_looks_like_arg()) -- everything before that word is
 * the decoration to strip. input_build_prefix_pattern() then turns
 * that decoration into a reusable pattern (runs of non-space,
 * non-punctuation collapsed to a single '*', every space/punctuation
 * character kept literal, since those are what actually carries a
 * decoration format's fixed shape -- brackets, colons, dashes, ...),
 * and input_match_prefix_pattern() applies that pattern to another
 * line to see if it independently reproduces the same split point --
 * see input_detect_prefix_pattern() for how the two combine. */

/**
 * @brief      is_command_b(), but safe to call on a token that's a
 *             slice of a larger buffer (e.g. straight out of an
 *             mmap()'d file) rather than its own NUL-terminated
 *             string. is_command_b()'s absolute-path branch ignores
 *             the length it's given and calls path_exists() directly
 *             on the pointer, which -- unlike its relative-path
 *             branch, which does copy exactly `len` bytes into its
 *             own buffer first -- means it reads until it happens to
 *             hit a real NUL byte, not just `len` bytes in.
 *
 * @param[in]  x  Token (need not be its own NUL-terminated string)
 * @param[in]  n  Length
 */
static bool
input_word_is_command(const char* x, size_t n) {
  stralloc tmp;
  bool ret;

  stralloc_init(&tmp);
  stralloc_copyb(&tmp, x, n);
  stralloc_nul(&tmp);
  ret = is_command_b(tmp.s, tmp.len) != 0;
  stralloc_free(&tmp);

  return ret;
}

/**
 * @brief      Whether a token looks like something that could plausibly
 *             be a real command-line argument, as opposed to a stray
 *             word from surrounding log/prose text (which is what a
 *             day's decoration in front of the real argument list is
 *             made of).
 *
 * @param[in]  x  Token
 * @param[in]  n  Length
 */
static bool
input_word_looks_like_arg(const char* x, size_t n) {
  if(n == 2 && byte_equal(x, 2, "->"))
    return false;

  if(byte_chr(x, n, '(') < n || byte_chr(x, n, ')') < n)
    return false;

  if(n > 0 && x[n - 1] == ',')
    return false;

  /* a bare word ending in ':' ("argv:", "Note:", ...) reads as prose,
   * not an argument -- but a flag/path that merely happens to end in
   * ':' (e.g. "-Wl,-rpath,/some/dir:") is fine, so only reject this
   * when the token doesn't otherwise look like one of those. */
  if(n > 0 && x[n - 1] == ':' && x[0] != '-' && x[0] != '/')
    return false;

  return true;
}

/**
 * @brief      Find the offset of the first token on a line that both
 *             resolves to a real command (@see is_command_b()) and is
 *             followed only by tokens that still look like plausible
 *             arguments -- the start of the line's actual argument
 *             list, with everything before it being decoration to
 *             strip.
 *
 * @param[in]  x  Line (no trailing \r\n)
 * @param[in]  n  Line length
 *
 * @return     Offset of the argument list, or (size_t)-1 if no token
 *             on this line qualifies
 */
static size_t
input_find_command_start(const char* x, size_t n) {
  size_t pos = 0;

  while(pos < n) {
    size_t i, start;

    if((i = scan_whitenskip_escaped(x + pos, n - pos)) == n - pos)
      break;

    pos += i;
    start = pos;
    i = scan_nonwhitenskip_escaped(x + pos, n - pos);

    if(input_word_is_command(x + start, i)) {
      size_t rest = start + i;
      bool all_args = true;

      while(rest < n) {
        size_t j, wstart;

        if((j = scan_whitenskip_escaped(x + rest, n - rest)) == n - rest)
          break;

        rest += j;
        wstart = rest;
        j = scan_nonwhitenskip_escaped(x + rest, n - rest);

        if(!input_word_looks_like_arg(x + wstart, j)) {
          all_args = false;
          break;
        }

        rest += j;
      }

      if(all_args)
        return start;
    }

    pos = start + i;
  }

  return (size_t)-1;
}

/**
 * @brief      Turn a confirmed decoration prefix into a reusable
 *             pattern: every run of characters that's neither
 *             whitespace nor punctuation (the "variable" parts -- a
 *             timestamp's digits, a pid, a process name, ...) becomes
 *             a single '*'; every space/punctuation character (the
 *             fixed shape -- brackets, colons, dashes, ...) is kept
 *             literal.
 *
 * @param[in]  prefix  Decoration text (before the argument list)
 * @param[in]  len     Length
 * @param      out     Pattern, built fresh (not appended to)
 */
static void
input_build_prefix_pattern(const char* prefix, size_t len, stralloc* out) {
  size_t i = 0;

  stralloc_zero(out);

  while(i < len) {
    unsigned char c = prefix[i];

    if(isspace(c) || ispunct(c)) {
      stralloc_catb(out, (const char*)&prefix[i], 1);
      i++;
    } else {
      size_t j = i;

      while(j < len && !isspace((unsigned char)prefix[j]) && !ispunct((unsigned char)prefix[j]))
        j++;

      stralloc_catb(out, "*", 1);
      i = j;
    }
  }
}

/**
 * @brief      Apply a pattern built by input_build_prefix_pattern()
 *             to another line: walk both in lockstep, a literal
 *             pattern byte requiring an exact match and a '*'
 *             consuming a run of non-space/non-punctuation characters
 *             (of any length, including zero) in the line.
 *
 * @param[in]  pattern  Pattern text
 * @param[in]  plen     Pattern length
 * @param[in]  x        Line to test
 * @param[in]  n        Line length
 *
 * @return     Number of leading bytes of `x` consumed by the match, or
 *             (size_t)-1 if the pattern doesn't match at all
 */
static size_t
input_match_prefix_pattern(const char* pattern, size_t plen, const char* x, size_t n) {
  size_t pi = 0, xi = 0;

  while(pi < plen) {
    if(pattern[pi] == '*') {
      while(xi < n && !isspace((unsigned char)x[xi]) && !ispunct((unsigned char)x[xi]))
        xi++;

      pi++;
    } else {
      if(xi >= n || x[xi] != pattern[pi])
        return (size_t)-1;

      xi++;
      pi++;
    }
  }

  return xi;
}

typedef struct {
  bool is_command;        /* argv[0] resolves to a real executable */
  bool has_dash_c;        /* has a bare "-c" token (compile-only) */
  bool has_source;        /* has a .c/.cc/.S/... source argument */
  bool has_object_or_lib; /* has a .o/.a argument, or a "-l..." flag */
  bool has_o_nonobj;      /* "-o NAME"/"-oNAME" where NAME isn't a .o -- a link/compile+link output */
} line_shape_t;

/**
 * @brief      Tokenize an already-de-prefixed line the same way
 *             input_process_line() does, and classify it as looking
 *             like a compile command, a link command, both (a single
 *             "cc -o prog prog.c" compile+link invocation), or
 *             neither.
 *
 * @param[in]  x      Argument list (decoration already stripped)
 * @param[in]  n      Length
 * @param      shape  Filled in with what was found
 */
static void
input_line_classify(const char* x, size_t n, line_shape_t* shape) {
  size_t idx = 0;

  byte_zero((char*)shape, sizeof(*shape));

  while(n > 0) {
    size_t i;

    if((i = scan_whitenskip_escaped(x, n)) == n)
      break;

    x += i;
    n -= i;

    i = scan_nonwhitenskip_escaped(x, n);

    if(idx == 0) {
      shape->is_command = input_word_is_command(x, i);
    } else if(i == 2 && byte_equal(x, 2, "-c")) {
      shape->has_dash_c = 1;
    } else if(i == 2 && byte_equal(x, 2, "-o")) {
      size_t j = scan_whitenskip_escaped(x + i, n - i);
      const char* name = x + i + j;
      size_t namelen = scan_nonwhitenskip_escaped(name, n - i - j);

      if(namelen && !is_object_b(name, namelen))
        shape->has_o_nonobj = 1;
    } else if(i > 2 && x[0] == '-' && x[1] == 'o') {
      if(!is_object_b(x + 2, i - 2))
        shape->has_o_nonobj = 1;
    } else if(is_source_b(x, i)) {
      shape->has_source = 1;
    } else if(is_object_b(x, i) || is_lib_b(x, i) || byte_ends(x, i, ".so") || (i > 2 && x[0] == '-' && x[1] == 'l')) {
      shape->has_object_or_lib = 1;
    }

    x += i;
    n -= i;
    idx++;
  }
}

static bool
input_line_skip_uninteresting(const char* x, size_t len) {
  return (len > 2 && x[0] == '-' && x[1] == '-') || byte_finds(x, len, "ing directory '") < len;
}

/**
 * @brief      Pre-read the whole --infile once to figure out, from its
 *             own content, whether its lines carry a decoration prefix
 *             in front of the actual argument list, and if so what
 *             pattern that decoration follows.
 *
 *             The first line where input_find_command_start() finds a
 *             split point becomes the anchor: its prefix (if any)
 *             seeds a candidate pattern via input_build_prefix_pattern().
 *             That candidate is trusted only once two things are both
 *             true: (1) it independently reproduces the same split
 *             point input_find_command_start() finds on some *other*
 *             line (input_match_prefix_pattern()'s result equals that
 *             line's own independently-found split), confirming the
 *             shape actually recurs rather than being a one-off; and
 *             (2) across the lines examined along the way, at least
 *             one looked like a compile command and at least one
 *             (possibly the same line) looked like a link command --
 *             see input_line_classify(). Requiring both guards against
 *             a single stray line (or a false command-word match
 *             inside a genuinely clean argument list) deciding the
 *             whole file's format.
 *
 * @param[in]  x    Whole file contents
 * @param[in]  n    Length
 * @param      out  Set to the confirmed pattern (possibly empty, if
 *                   lines turned out to already be clean argument
 *                   lists) when this returns true
 *
 * @return     true if a pattern was confirmed, false if no consistent
 *             decoration shape could be established (out is left
 *             untouched, callers should skip 0 bytes on every line)
 */
static bool
input_detect_prefix_pattern(const char* x, size_t n, stralloc* out) {
  const char* p = x;
  size_t remaining = n;
  bool have_anchor = false, validated = false, saw_compile = false, saw_link = false;

  while(remaining > 0) {
    size_t i = scan_lineskip_escaped(p, remaining);
    size_t len = byte_trimr(p, i, "\r\n", 2);

    if(len > 0 && !input_line_skip_uninteresting(p, len)) {
      size_t split = input_find_command_start(p, len);

      if(split != (size_t)-1) {
        line_shape_t shape;

        input_line_classify(p + split, len - split, &shape);

        if(shape.is_command && shape.has_source)
          saw_compile = true;

        if(shape.is_command && !shape.has_dash_c && (shape.has_object_or_lib || shape.has_o_nonobj))
          saw_link = true;

        if(!have_anchor) {
          input_build_prefix_pattern(p, split, out);
          have_anchor = true;

          if(split == 0)
            validated = true; /* no decoration at all -- trivially consistent */

        } else if(!validated) {
          if(input_match_prefix_pattern(out->s, out->len, p, len) == split)
            validated = true;
        }

        if(have_anchor && validated && saw_compile && saw_link)
          return true;
      }
    }

    p += i;
    remaining -= i;
  }

  return false;
}

/**
 * @brief      Process commands from file
 *
 * @param[in]  infile  Input file
 * @param      all     All target
 *
 * @return    -1 on error, 0 on success
 */
int
input_process_file(const char* infile, target* all) {
  const char* x;
  size_t n, line = 1;
  int ret = 0;
  stralloc prefix_pattern;
  bool have_prefix_pattern;

  path_dirname(infile, &dirs.this.sa);
  strlist_nul(&dirs.this);

  if((x = mmap_read(infile, &n))) {
    /* figure out, from the file's own content, whether its lines
     * carry a decoration prefix in front of the actual argument list
     * and if so what shape it follows -- then process the file for
     * real, from the beginning, applying whatever was found
     * consistently to every line (see input_detect_prefix_pattern()). */
    stralloc_init(&prefix_pattern);
    have_prefix_pattern = input_detect_prefix_pattern(x, n, &prefix_pattern);

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
          size_t len = byte_trimr(x, i, "\r\n", 2);
          size_t skip = 0;

          if(have_prefix_pattern) {
            size_t matched = input_match_prefix_pattern(prefix_pattern.s, prefix_pattern.len, x, len);

            /* the pattern's own "*"s can't split a word in half (each
             * one always consumes a *complete* run of non-space/
             * non-punct characters, never stops partway through one),
             * but a line whose decoration doesn't perfectly conform
             * to the one shape the pattern was derived from could
             * still make it land on the wrong boundary entirely --
             * e.g. a derived pattern ending in a bare "* * ", with no
             * distinguishing punctuation right before the split,
             * could keep matching past the true boundary on a
             * differently-shaped line and eat into its real argv[].
             * Guard against that the same way input_find_command_start()
             * itself does: only trust the match if the word sitting
             * right at the landing spot actually resolves to a real
             * command -- if it doesn't, this line's decoration wasn't
             * really this shape after all, so leave it unstripped
             * rather than risk a silently wrong split. */
            if(matched != (size_t)-1) {
              size_t wlen = scan_nonwhitenskip_escaped(x + matched, len - matched);

              if(wlen > 0 && input_word_is_command(x + matched, wlen))
                skip = matched;
            }
          }

          if((ret = input_process_line(x + skip, len - skip, infile, line)) < 0)
            break;
        }
      }

      x += i;
      n -= i;
      line += ln;
    }

    mmap_unmap(x, n);
    stralloc_free(&prefix_pattern);
  }

  if(set_size(&common_flags)) {
#ifdef DEBUG_OUTPUT
    buffer_puts(debug_buf, "Common flags: ");
    buffer_putsl(debug_buf, &common_flags_list, " ");
    buffer_putnlflush(debug_buf);
#endif

    var_t* v = var_list("COMMON_FLAGS", 0);

    /*stralloc_copyb(&v->value.sa, common_flags_list.sa.s, common_flags_list.sa.len);
    stralloc_nul(&v->value.sa);*/
    strlist_copy(&v->value, &common_flags_list);
    v->value.sep = ' ';

    /*    var_set_set("COMMON_FLAGS", &common_flags);*/
    const char* val = var_get("COMMON_FLAGS");

#ifdef DEBUG_OUTPUT
    buffer_puts(debug_buf, "Common flags (2): ");
    buffer_puts(debug_buf, val);
    buffer_putnlflush(debug_buf);
#endif
  }

  input_process_rules(all);

  return ret < 0 ? ret : 0;
}
