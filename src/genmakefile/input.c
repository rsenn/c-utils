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
static void
builddir_enter(const char* x, size_t len) {
  stralloc tmp;

  stralloc_init(&tmp);
  strarray_emplace_sa(&dirstack, &dirs.build.sa);
  stralloc_copyb(&dirs.build.sa, x, len);

  path_relative_b(dirs.build.sa.s, dirs.build.sa.len, &tmp);
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
static void
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
    stralloc_free(&dirs.build.sa);

    dirs.build.sa.s = strarray_pop(&dirstack);
    dirs.build.sa.len = dirs.build.sa.s ? str_len(dirs.build.sa.s) : 0;
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
    path_append(dirs.build.sa.s, dirs.build.sa.len, out);
    path_append(y, len, out);
    stralloc_nul(out);
    path_relative_to(out->s, dirs.out.sa.s, out);
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
  path_relative_to_b(dirs.out.sa.s, dirs.out.sa.len, dirs.build.sa.s, dirs.build.sa.len, &rel);

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
        strlist_push(&flags, "-o");
        strlist_push(&flags, path.s);
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
        stralloc_zero(&path);
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
        stralloc_zero(&path);
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
        buffer_putm_internal(buffer_2, GREEN256, __func__, NC, " File '", 0);
        buffer_put(buffer_2, y, len);
        buffer_puts(buffer_2, "");
        buffer_putnlflush(buffer_2);
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
          buffer_putm_internal(buffer_2, GREEN256, __func__, NC, " File '", 0);
          buffer_put(buffer_2, tmp.s, tmp.len);
          buffer_puts(buffer_2, "'");
          buffer_putnlflush(buffer_2);
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
      if(set_has(&fs, x, n))
        strlist_pushb_unique(&common_flags_list, x, n);
    }
  }

  if(out.len) {
    strlist_removeb(&files, out.s, out.len);
    path_canonical_sa(&out);
    strlist_removeb(&files, out.s, out.len);
  }

  if(out.len == 0 && compile) {
    stralloc_copys(&out, path_basename2(files.sa.s, byte_chr(files.sa.s, files.sa.len, files.sep)));

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
  buffer_putm_internal(buffer_2, GREEN256, __func__, NC, " '", 0);
  buffer_putsa(buffer_2, cmd);
  buffer_puts(buffer_2, "'\treldir = ");
  buffer_putsa(buffer_2, &rel);
  buffer_puts(buffer_2, "\n\tbuilddir = ");
  buffer_putsa(buffer_2, &dirs.build.sa);
  buffer_puts(buffer_2, "\n\toutdir = ");
  buffer_putsa(buffer_2, &dirs.out.sa);
  buffer_puts(buffer_2, "\n\tcmd = ");
  buffer_putsa(buffer_2, cmd);
  buffer_puts(buffer_2, "\n\tcompile = ");
  buffer_putlong(buffer_2, compile);
  buffer_puts(buffer_2, "\n\tlink = ");
  buffer_putlong(buffer_2, link);
  buffer_puts(buffer_2, "\n\tlib = ");
  buffer_putlong(buffer_2, lib);
  buffer_puts(buffer_2, "\n\targs =\n\t\t");
  buffer_putsl(buffer_2, &args, "\n\t\t");
  buffer_puts(buffer_2, "\n\tfiles =\n\t\t");
  buffer_putsl(buffer_2, &files, "\n\t\t");
  buffer_putnlflush(buffer_2);
#endif

  if(out.len == 0) {
    stralloc tmp;

    stralloc_init(&tmp);
    // path_normalize_b(files.tmp.s, byte_chr(files.tmp.s, files.tmp.len, files.sep), &tmp);

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
  buffer_puts(buffer_2, file);
  buffer_puts(buffer_2, ":");
  buffer_putulong(buffer_2, line);
  buffer_puts(buffer_2, ",\n\tobjects = ");
  buffer_putulong(buffer_2, objects);
  buffer_puts(buffer_2, ",\n\toutput = ");
  buffer_putsa(buffer_2, &out);
  buffer_puts(buffer_2, ",\n\tcmd = ");
  buffer_putsa(buffer_2, cmd);
  buffer_puts(buffer_2, ",\n\tfiles =\n\t\t");
  buffer_putsl(buffer_2, &files, ",\n\t\t");
  buffer_putnlflush(buffer_2);
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
    buffer_puts(buffer_2, "Create ");
    buffer_puts(buffer_2, lib ? "lib" : link ? "link" : compile ? "compile" : "other");
    buffer_puts(buffer_2, " RULE\n\toutput = ");
    buffer_putsa(buffer_2, &out);

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
        buffer_puts(buffer_2, "\trule = '");
        buffer_puts(buffer_2, rule->name);
        buffer_puts(buffer_2, "'\n\tprereq = ");
        buffer_putset(buffer_2, &rule->prereq, " ", 1);
        buffer_putnlflush(buffer_2);
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
  buffer_putm_internal(buffer_2, "end ", GREEN256, __func__, NC, 0);
  buffer_putnlflush(buffer_2);
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
  buffer_putm_internal(buffer_2, PINK256, __func__, NC, " \"", 0);
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

#ifdef DEBUG_OUTPUT_
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

#ifdef DEBUG_OUTPUT_
  buffer_putm_internal(buffer_2, "end ", PINK256, __func__, NC, 0);
  buffer_putnlflush(buffer_2);
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
    buffer_putm_internal(debug_buf, YELLOW256, __func__, NC, " Rule: ", name, link ? " (link)" : compile ? " (compile)" : 0, NULL);
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
  buffer_puts(buffer_2, "outdir:\n\t");
  buffer_putsa(buffer_2, &outdir.sa);
  buffer_putnlflush(buffer_2);
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
  strlist_at_sa(&args, &cc->value.sa, 0);

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
      buffer_puts(buffer_2, "Prereq: ");
      buffer_putset(buffer_2, &rule->prereq, " ", 1);
      buffer_putnlflush(buffer_2);
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
    buffer_putm_internal(buffer_2, YELLOW256, __func__, NC, " Rule: ", name, compile ? " (compile)" : link ? " (link)" : "", "\n\t", NULL);
    buffer_putsa(buffer_2, &rule->recipe);
    buffer_putnlflush(buffer_2);
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
          if((ret = input_process_line(x, byte_trimr(x, i, "\r\n", 2), infile, line)) < 0)
            break;
        }
      }

      x += i;
      n -= i;
      line += ln;
    }

    mmap_unmap(x, n);
  }

  if(set_size(&common_flags)) {

#ifdef DEBUG_OUTPUT
    buffer_puts(debug_buf, "Common flags: ");
    buffer_putset(debug_buf, &common_flags, " ", 1);
    buffer_putnlflush(debug_buf);
#endif

    var_t* v = var_list("COMMON_FLAGS", 0);

    stralloc_copyb(&v->value.sa, common_flags_list.sa.s, common_flags_list.sa.len);
    stralloc_nul(&v->value.sa);

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
