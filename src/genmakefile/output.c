#include "genmakefile.h"
#include "../../lib/debug.h"
#include "output.h"
#include "xc8.h"
#include "compiler.h"
#include "ansi.h"

int output_width = 40;
make_capabilities_t make_capabs = 0;

/**
 * @brief      Extract variables from buffer
 *
 * @param[in]  x     Buffer
 * @param[in]  len   Length
 * @param      s     Output set
 */
static void
extract_vars(const char* x, size_t len, set_t* out) {
  size_t i;
  set_clear(out);

  for(i = 0; i < len; ++i) {
    const char* p = &x[i];

    if(i + 4 <= len && *p == '$' && p[1] == '(') {
      size_t vlen;

      i += 2;
      vlen = byte_chr(&x[i], len - i, ')');
      set_add(out, &x[i], vlen);
      i += vlen;

      continue;
    }
  }
}

/**
 * @brief      Output variable
 *
 * @param      b       { parameter_description }
 * @param      vars    The variables
 * @param[in]  name    The name
 * @param[in]  serial  The serial
 * @param[in]  tool    Build tool
 */
void
output_var(buffer* b, MAP_T* vars, const char* name, int serial, build_tool_t tool) {
  stralloc v;
  var_t* var;
  set_t refvars;
  const char* ref;
  size_t len;
  set_iterator_t it;
  MAP_PAIR_T t = 0;

  set_init(&refvars);

  if(MAP_SEARCH(*vars, name, str_len(name) + 1, &t)) {
    var = MAP_ITER_VALUE(t);

    if(var->serial == serial)
      return;

    stralloc_init(&v);

    if(var->value.sa.s) {
      stralloc_copys(&v, MAP_ITER_KEY(t));

      if(tool == TOOL_NINJA)
        stralloc_lower(&v);

      stralloc_nul(&v);
      set_clear(&refvars);

      if(!infile) {
        extract_vars(var->value.sa.s, var->value.sa.len, &refvars);

        set_foreach(&refvars, it, ref, len) {
#ifdef DEBUG_OUTPUT
          buffer_putm_internal(debug_buf, "Var ", name, " ref: ", ref, NULL);
          buffer_putnlflush(debug_buf);
#endif
          output_var(b, vars, ref, serial, tool);
        }
      }

      if(tool == TOOL_BATCH)
        buffer_putm_internal(b, "@SET ", v.s, "=", NULL);
      else if(tool == TOOL_SHELL)
        buffer_putm_internal(b, v.s, "=\"", NULL);
      else
        buffer_putm_internal(b, v.s, " = ", NULL);

      var->serial = serial;
      {
        stralloc u;
        stralloc_init(&u);

        if(tool == TOOL_NINJA)
          stralloc_copy(&u, &var->value.sa);
        else
          strlist_joinq(&var->value, &u, ' ', '"');

        if(tool == TOOL_NINJA || tool == TOOL_SHELL) {
          stralloc_zero(&v);
          /* shell variable names are never lowercased (unlike ninja's,
           * see stralloc_lower(&v) above) -- lowering a nested $(NAME)
           * reference here would point at a shell variable that was
           * never defined. */
          transform_subst_sa(&u, &v, "$", "", tool == TOOL_NINJA);
        } else if(tool == TOOL_BATCH) {
          stralloc_zero(&v);
          transform_subst_sa(&u, &v, "%", "%", 1);
        } else {
          stralloc_copy(&v, &u);
        }

        stralloc_free(&u);
      }

      buffer_putsa(b, &v);

      if(tool == TOOL_SHELL)
        buffer_putc(b, '"');

      buffer_putnl(b, 0);
      buffer_flush(b);
    }

    stralloc_free(&v);

    MAP_DELETE(*vars, MAP_ITER_KEY(t), str_len(MAP_ITER_KEY(t)));
  }

  set_free(&refvars);
}

/**
 * @brief       Output all variables
 *
 * @param      b         { parameter_description }
 * @param      vars      The variables
 * @param      varnames  The varnames
 * @param[in]  tool      Build tool
 */
void
output_all_vars(buffer* b, MAP_T* vars, strlist* varnames, build_tool_t tool) {
  const char* name;
  static int serial = 0;

  stralloc_nul(&varnames->sa);
  ++serial;

  strlist_foreach_s(varnames, name) {
#ifdef DEBUG_OUTPUT
    if(dump_level >= 2) {
      buffer_putm_internal(debug_buf, "Outputting variable '", name, "'.", 0);
      buffer_putnlflush(debug_buf);
    }
#endif

    output_var(b, vars, name, serial, tool);
  }

  buffer_putnl(b, 1);
}

/**
 * @brief      Output a make rule
 *
 * @param      b                Output buffer
 * @param      rule             The rule
 * @param[in]  tool             Build tool
 * @param[in]  quote            Quote
 * @param[in]  psa              Path separator for arguments
 * @param[in]  psm              Path separator in Makefile
 * @param[in]  sep              Inline separator
 */
void
output_make_rule(buffer* b, target* rule, build_tool_t tool, const char quote[], char psa, char psm, const char* sep) {
  const char* x;
  stralloc output, sa, name;
  size_t n, num_prereqs = set_size(&rule->prereq), line_start = 0;

  debug_nl = "\n";

  stralloc_init(&output);
  stralloc_init(&sa);
  stralloc_init(&name);

  stralloc_copys(&name, rule->name);

#ifdef DEBUG_OUTPUT
  if(dump_level >= 2) {
    buffer_puts(debug_buf, "RULE\n  " YELLOW256 "name" NC ": ");
    buffer_puts(debug_buf, rule->name);
    buffer_puts(debug_buf, "\n  " PURPLE256 "recipe" NC ": ");
    buffer_putsa(debug_buf, &rule->recipe);
    buffer_puts(debug_buf, "\n  " BLUE256 "output" NC ":\n\t");
    buffer_putset(debug_buf, &rule->output, "\n\t", 2);
    buffer_puts(debug_buf, "\n  " GREEN256 "prereq" NC ":\n\t");
    buffer_putset(debug_buf, &rule->prereq, "\n\t", 2);

    if(array_length(&rule->deps, sizeof(target*))) {
      buffer_puts(debug_buf, "\n  deps" NC ": ");
      print_rule_deps(debug_buf, rule);
    }

    buffer_putnlflush(debug_buf);
  }
#endif

  if(rule->phony || (num_prereqs == 0 && str_diffn(rule->name, dirs.work.sa.s, dirs.work.sa.len) &&
                     !rule->name[str_chr(rule->name, psm)] && str_end(rule->name, ":"))) {
    buffer_putm_internal(b, ".PHONY: ", rule->name, newline, NULL);
  }

  stralloc_zero(&sa);
  set_join(&rule->output, " \\\n", &sa);

#ifdef DEBUG_OUTPUT_
  buffer_puts(debug_buf, "RULE OUTPUT: ");
  buffer_putsa_escaped(debug_buf, &sa, &fmt_escapecharcontrol);
  buffer_putnlflush(debug_buf);
#endif

  strlist_foreach(&rule->vars, x, n) {
    buffer_putsa(b, &sa);
    buffer_puts(b, ": ");
    buffer_put(b, x, n);
    buffer_puts(b, "\n\n");
  }

  stralloc_zero(&output);
  stralloc_nul(&output);

  bool pattern_rule = (make_capabs & MAKE_RULE_PATTERN) && num_prereqs == 0 && str_contains(rule->name, "%");

  /* a static pattern rule "a.o b.o: %.o: %.c" needs the outputs; without them it is the plain "%.o: %.c" */
  if(pattern_rule && sa.len == 0) {
    stralloc_cats(&sa, rule->name);
  } else if(pattern_rule) {
    stralloc_cats(&sa, ": ");
    stralloc_cats(&sa, rule->name);

  } else {
    if(!set_has_s(&rule->output, rule->name)) {
      stralloc_cats(&sa, " \\\n");
      stralloc_cats(&sa, rule->name);
    }
  }

  if(sa.len > 0 && rule->outputs)
    stralloc_cat(&output, &sa);
  else
    stralloc_cats(&output, rule->name);

  if(!pattern_rule)
    stralloc_catc(&output, ':');

  if(num_prereqs) {
    const char* str;
    size_t len;
    bucket_t* it;

    set_foreach_ordered(&rule->prereq, it, str, len) {
      if(stralloc_endb(&output, str, len))
        continue;

      bool break_line = (output.len - line_start) + len + 1 + 2 >= output_width;

      stralloc_cats(&output, break_line ? OUTPUT_PREREQ_SEPARATOR : " ");

      if(break_line)
        line_start += scan_lineskip(&output.s[line_start], output.len - line_start);

      stralloc_catb(&output, str, len);
    }
  }

  /*  bool no_explicit = rule->type == COMPILE && (make_capabs & MAKE_RULE_PATTERN);*/

  if(rule->recipe.s /* && !no_explicit*/) {
    stralloc cmd;

    stralloc_init(&cmd);

    if(cfg.sys.os == OS_WIN && rule->recipe.s == commands.mkdir.s && str_chr(rule->name, '/') < str_len(rule->name)) {
      /* IF NOT EXIST $@ MKDIR $@ only tests/creates the leaf component --
       * for a multi-component directory that doesn't already exist, emit
       * one guarded MKDIR per path component instead of a single $@. */
      strlist dirs;
      size_t i, n;

      strlist_init(&dirs, '/');
      strlist_froms(&dirs, rule->name, '/');
      n = strlist_count(&dirs);

      for(i = 1; i <= n; ++i) {
        strlist r = strlist_range(&dirs, 0, i);
        size_t rlen = r.sa.len, p;

        if(rlen && r.sa.s[rlen - 1] == '/')
          rlen--;

        if(i > 1) {
          stralloc_catc(&cmd, '\n');
          stralloc_catc(&cmd, '\t');
        }

        /* IF NOT EXIST/MKDIR run under cmd.exe regardless of the make
         * tool (mingw32-make included, where psa stays '/'), and cmd.exe
         * doesn't accept forward slashes here -- force backslashes. */
        stralloc_cats(&cmd, "IF NOT EXIST ");
        p = cmd.len;
        stralloc_catq(&cmd, r.sa.s, rlen, quote);
        byte_replace(&cmd.s[p], cmd.len - p, '/', '\\');

        stralloc_cats(&cmd, " MKDIR ");
        p = cmd.len;
        stralloc_catq(&cmd, r.sa.s, rlen, quote);
        byte_replace(&cmd.s[p], cmd.len - p, '/', '\\');
      }

      strlist_free(&dirs);
    } else if(rule->type == LINK && (make_capabs & MAKE_RULE_IMPLICIT))
      stralloc_copy(&cmd, &commands.link);
    else
      /* XXX

      if(infile && (signed)rule->type >= 0)
        stralloc_copy(&cmd, &commands.v[rule->type]);
      else*/
      rule_command(rule, &cmd, tool, quote, psa, sep, tools.make);

    stralloc_remove_all(&cmd, "\0", 1);

    /* the terminator of an inline file has to start its line */
    if(make_end_inline && make_end_inline[0] == '\r' && str_start(tools.make, "borland"))
      stralloc_replaces(&cmd, "\n\t|", "\n|");

    if(!stralloc_starts(&cmd, newline)) {
      stralloc_catc(&output, '\n');
      stralloc_catc(&output, '\t');
    }

    stralloc_cat(&output, &cmd);
    stralloc_catc(&output, '\n');

    if(str_end(rule->name, ":")) {
      bucket_t* b;

      stralloc_catc(&output, '\n');
      stralloc_catc(&output, '\n');

      for(b = rule->prereq.list; b; b = b->list_next) {
        stralloc_catc(&output, ' ');
        stralloc_catb(&output, b->value, b->size);
      }

      stralloc_cats(&output, " :");
      stralloc_catc(&output, '\n');
    }

    stralloc_free(&cmd);
  }

  stralloc_catc(&output, '\n');

  if(str_equal(tools.make, "gmake")) {
    const char* bd;
    size_t bdlen;

    if((bd = var_get(builddir_varname)) && (bdlen = str_len(bd)) > 0 && stralloc_starts(&output, bd)) {
      stralloc repl;

      stralloc_init(&repl);
      stralloc_copys(&repl, "$(");
      stralloc_cats(&repl, builddir_varname);
      stralloc_cats(&repl, ")");

      stralloc_remove(&output, 0, bdlen);
      stralloc_insertb(&output, repl.s, 0, repl.len);

      stralloc_free(&repl);
    }
  }

  buffer_putsa(b, &output);
  buffer_flush(b);

  stralloc_free(&name);
  stralloc_free(&sa);
  stralloc_free(&output);
}

/**
 * @brief      Output a Ninja target
 *
 * @param      b     Output buffer
 * @param      rule  The rule
 * @param[in]  psa   Path separator for arguments
 */
void
output_ninja_target(buffer* b, target* rule, char psa) {
  const char* rule_name = 0;
  stralloc obj_dir;

  stralloc_init(&obj_dir);

  if(xc8_is_vectors_rule(rule))
    rule_name = "ihextool_vectors";
  else if(rule_is_compile(rule) || rule->recipe.s == commands.compile.s)
    rule_name = "cc";
  else if(str_end(rule->name, exts.lib) || rule->recipe.s == commands.lib.s)
    /* checked ahead of rule_is_link(): a plain program's link rule can
     * also have only-object prereqs, which rule_is_lib()'s fallback
     * heuristic (no extension check) would also match */
    rule_name = "lib";
  else if(rule_is_link(rule) || rule->recipe.s == commands.link.s)
    rule_name = "link";

  if(rule_name) {
    stralloc path;
    /* a "link" rule's own output is the linked binary, which lives
     * under dirs.bin (dirs.obj by default, unless -k/--bindir was
     * given) -- everything else ("cc"/"lib" outputs, and a link rule's
     * own prereqs below) still lives under dirs.obj. */
    const char* out_dir =
        str_equal(rule_name, "link") || str_equal(rule_name, "ihextool_vectors") ? dirs.bin.sa.s : dirs.obj.sa.s;

    stralloc_init(&path);
    {
      size_t n;
      const char* x = set_first(&rule->output, &n);

      if(x)
        stralloc_copyb(&path, x, n);
      stralloc_nul(&path);
    }

    stralloc_replaces(&path, out_dir, "$objdir");

    buffer_puts(b, "build ");
    buffer_putsa(b, &path);

    /* xc8-cc writes the .hex when it links the .elf: an implicit output */
    if(xc8_is_cc() && str_equal(rule_name, "link") && path.len > 4 && !byte_diff(path.s + path.len - 4, 4, ".elf")) {
      buffer_puts(b, " | ");
      buffer_put(b, path.s, path.len - 4);
      buffer_puts(b, ".hex");
    }

    buffer_puts(b, ": ");
    buffer_puts(b, rule_name);
    buffer_puts(b, " ");

    stralloc_zero(&path);

    path_rel_sa(&obj_dir, dirs.out.sa.s, dirs.out.sa.len, dirs.work.sa.s, dirs.work.sa.len);

    {
      const char* x;
      size_t n, i = 0;
      bucket_t* it;
      stralloc tmp, outdir;

      stralloc_init(&tmp);
      stralloc_init(&outdir);
      path_cat_sa(&outdir, dirs.this.sa.s, dirs.this.sa.len, dirs.out.sa.s, dirs.out.sa.len);

      set_foreach_ordered(&rule->prereq, it, x, n) {
        if(i)
          stralloc_catc(&path, ' ');

        stralloc_catb(&path, x, n);
        stralloc_zero(&tmp);
        i++;
      }

      stralloc_free(&tmp);
      stralloc_free(&outdir);
    }

    stralloc_nul(&path);

    stralloc_replacec(&path, psa == '/' ? '\\' : '/', psa == '/' ? '/' : '\\');
    stralloc_replaces(&path, dirs.obj.sa.s, "$objdir");

    buffer_putsa(b, &path);
    buffer_putnlflush(b);

    stralloc_free(&path);
  }

  stralloc_free(&obj_dir);
}

/**
 * @brief      Output a Ninja build rule
 *
 * @param      b     Output buffer
 * @param      name  Rule name
 * @param      cmd   Command
 */
void
output_ninja_rule(buffer* b, const char* name, const stralloc* cmd) {
  stralloc out;

  stralloc_init(&out);

  buffer_putm_internal(b, "rule ", name, "\n  command = ", NULL);

  transform_subst_sa(cmd, &out, "$", "", 1);

  stralloc_replaces(&out, "$@", "$out");
  stralloc_replaces(&out, "$<", "$in");
  stralloc_replaces(&out, "$^", "$in");

  stralloc_remove_all(&out, "\"", 1);

  stralloc_removesuffixs(&out, newline);
  stralloc_removesuffixs(&out, "\r");

  buffer_putsa(b, &out);
  buffer_putsflush(b, newline);

  stralloc_free(&out);
}

/**
 * @brief      Output the rule set
 *
 * @param      b                Output buffer
 * @param[in]  tool             Build tool
 * @param[in]  quote            Quote arguments
 * @param[in]  psa              Path separator for arguments
 * @param[in]  psm              Path separator for Makefile
 * @param[in]  sep              Make separator inline
 */
void
output_all_rules(buffer* b, build_tool_t tool, const char quote[], char psa, char psm, const char* sep) {
  MAP_PAIR_T t;

  MAP_FOREACH(rule_map, t) {
    const char* name = MAP_ITER_KEY(t);
    target* rule = MAP_ITER_VALUE(t);

    if(!cmd_libs && str_end(name, ".a"))
      continue;

    if(rule->disabled)
      continue;

#ifdef DEBUG_OUTPUT
    if(dump_level >= 2) {
      buffer_puts(debug_buf, "Outputting rule '");
      buffer_puts(debug_buf, name);
      buffer_putc(debug_buf, '\'');
      buffer_putnlflush(debug_buf);
    }
#endif

    if(tool == TOOL_NINJA)
      output_ninja_target(b, rule, psa);
    else
      output_make_rule(b, rule, tool, quote, psa, psm, sep);
  }
}

/**
 * @brief      Output a script
 *
 * @param      b                Output buffer
 * @param      rule             The rule
 * @param[in]  tool             Build tool
 * @param[in]  quote            The quote arguments
 * @param[in]  psa              The psa
 * @param[in]  sep              The make separator inline
 */
void
output_script(buffer* b, target* rule, build_tool_t tool, const char quote[], char psa, const char* sep) {
  static uint32 serial;
  char* x;
  size_t n;
  set_iterator_t it;
  int flush = 0;

  if(rule == NULL) {
    /* decrement (like rule_dep_serial in rule.c) rather than increment --
     * target_s.serial doubles as rule_get()'s ascending creation-order
     * stamp (1, 2, 3, ...), so an ascending visited-marker here would
     * eventually collide with a real rule's creation serial and make
     * output_script() think it had already emitted that rule the very
     * first time it reached it, silently dropping its recipe. */
    flush = 1;
    --serial;
    rule = rule_get("all");
    --serial;

    /* emit every directory-creation rule up front, before anything
     * that might write into it. generate_auxiliary_rules() (genmakefile.c)
     * only ever adds a directory's mkdir rule as a *sibling*
     * prerequisite of "all", not as a real dependency edge of the
     * rule(s) that actually write into that directory (a real edge
     * would corrupt rule_command()'s use of rule->prereq as the
     * recipe's own input-file list, e.g. an archive rule's "$^").
     * make/ninja resolve this via their own dependency-graph
     * scheduling regardless of "all"'s prereq order; output_script()
     * has no such thing -- its only ordering signal between two
     * siblings under "all" is set_foreach()'s unspecified iteration
     * order, so without this the first rule to write into a given
     * directory can run before that directory's mkdir. */
    {
      MAP_PAIR_T t;

      MAP_FOREACH(rule_map, t) {
        target* dir_rule = MAP_ITER_VALUE(t);

        if(dir_rule->recipe.s == commands.mkdir.s && dir_rule->serial != serial)
          output_script(b, dir_rule, tool, quote, psa, sep);
      }
    }
  }

  if(rule->serial == serial)
    return;

  if(!rule->name[str_chr(rule->name, '%')])

    if(rule->recipe.s != commands.compile.s)
      buffer_putm_internal(
          b, newline, tool == TOOL_BATCH ? "REM" : "#", " Rules for '", rule->name, "'", newline, NULL);

  set_foreach(&rule->prereq, it, x, n) {
    target* dep = rule_find_b(x, n);

    if(!dep || dep->serial == serial)
      continue;

    output_script(b, dep, tool, quote, psa, sep);
  }

  if(array_length(&rule->objs, sizeof(target*))) {
    target** tptr;

    array_foreach_t(&rule->objs, tptr) {
      target* dep = *tptr;

      if(dep == 0 || dep->serial == serial)
        continue;

      output_script(b, dep, tool, quote, psa, sep);
    }
  }

  if(rule->recipe.len) {
    stralloc cmd;

    stralloc_init(&cmd);

    rule_command(rule, &cmd, tool, quote, psa, sep, tools.make);
    buffer_putsa(b, &cmd);

    /* an empty recipe (the rule "all") has no command to test */
    if(tool == TOOL_BATCH) {
      size_t k = 0;

      while(k < cmd.len && (cmd.s[k] == ' ' || cmd.s[k] == '\t' || cmd.s[k] == '\r' || cmd.s[k] == '\n'))
        ++k;

      if(k < cmd.len)
        buffer_puts(b, " || GOTO FAIL");
    }

    stralloc_free(&cmd);
  }

  if(str_equal(rule->name, "all")) {
    if(tool == TOOL_BATCH)
      buffer_putm_internal(b,
                           newline,
                           ":SUCCESS",
                           newline,
                           "ECHO Done.",
                           newline,
                           "GOTO QUIT",
                           newline,
                           newline,
                           ":FAIL",
                           newline,
                           "ECHO Fail.",
                           newline,
                           newline,
                           ":QUIT",
                           newline,
                           0);
  }

  buffer_putnl(b, flush);
  rule->serial = serial;
}

/**
 * @brief      Output the switches of the sources: for each name a conditional
 *             of the sources asks about, a definition that is added to DEFS
 *             when the build is given a value for that name, e.g.
 *             `make USE_UART=1`. Without one the name stays undefined.
 *
 * @param      b         Output buffer
 * @param[in]  tool      Build tool
 * @param      switches  The names
 */
void
output_switches(buffer* b, build_tool_t tool, const strarray* switches) {
  char** p;
  int bang = str_start(tools.make, "borland") || str_start(tools.make, "bmake") || str_start(tools.make, "nmake") ||
             str_start(tools.make, "po");

  if(!strarray_size(switches) || tool == TOOL_NINJA)
    return;

  if(tool == TOOL_BATCH)
    buffer_putm_internal(b, "REM Give a name below a value to define it, e.g. SET USE_X=1", newline, NULL);
  else if(tool == TOOL_SHELL)
    buffer_putm_internal(b, "# Give a name below a value to define it, e.g. USE_X=1", newline, NULL);
  else
    buffer_putm_internal(
        b, "# Give a name below a value to define it, e.g. ", bang ? "make -DUSE_X=1" : "make USE_X=1", newline, NULL);

  strarray_foreach(switches, p) {
    const char* n = *p;

    if(tool == TOOL_BATCH)
      buffer_putm_internal(b, "IF DEFINED ", n, " SET DEFS=%DEFS% -D", n, "=%", n, "%", newline, NULL);
    else if(tool == TOOL_SHELL)
      buffer_putm_internal(b, "if [ -n \"$", n, "\" ]; then DEFS=\"$DEFS -D", n, "=$", n, "\"; fi", newline, NULL);
    else if(bang)
      buffer_putm_internal(
          b, "!ifdef ", n, newline, "DEFS = $(DEFS) -D", n, "=$(", n, ")", newline, "!endif", newline, NULL);
    else
      buffer_putm_internal(b,
                           n,
                           " ?=",
                           newline,
                           "ifneq ($(",
                           n,
                           "),)",
                           newline,
                           "DEFS += -D",
                           n,
                           "=$(",
                           n,
                           ")",
                           newline,
                           "endif",
                           newline,
                           NULL);
  }

  buffer_putnl(b, 0);
  buffer_flush(b);
}

/**
 * @brief      Output the optional packages
 *
 * A package is enabled by giving its variable (LIBZ) a value: the library
 * names. LIBZ_INCLUDEDIR and LIBZ_LIBDIR add the search paths.
 *
 * @param      b     Output buffer
 * @param      tool  Build tool
 * @param      deps  Entries "MACRO=VARIABLE"
 */
void
output_deps(buffer* b, build_tool_t tool, const strarray* deps) {
  char** p;
  int bang = str_start(tools.make, "borland") || str_start(tools.make, "bmake") || str_start(tools.make, "nmake") ||
             str_start(tools.make, "po");

  if(!strarray_size(deps) || tool == TOOL_NINJA)
    return;

  if(tool == TOOL_BATCH)
    buffer_putm_internal(
        b,
        "REM Set a package to its library names to use it, with <NAME>_INCLUDEDIR and <NAME>_LIBDIR for the paths",
        newline,
        NULL);
  else if(tool == TOOL_SHELL)
    buffer_putm_internal(
        b,
        "# Set a package to its library names to use it, with <NAME>_INCLUDEDIR and <NAME>_LIBDIR for the paths",
        newline,
        NULL);
  else
    buffer_putm_internal(b,
                         "# Set a package to its library names to use it, e.g. ",
                         bang ? "make -DLIBZ=z" : "make LIBZ=z",
                         newline,
                         "# <NAME>_INCLUDEDIR and <NAME>_LIBDIR add the paths",
                         newline,
                         NULL);

  strarray_foreach(deps, p) {
    size_t n = str_chr(*p, '=');
    char* macro = str_ndup(*p, n);
    const char* v = *p + n + 1;
    stralloc inc, lib, dir;

    stralloc_init(&inc);
    stralloc_init(&lib);
    stralloc_init(&dir);
    stralloc_catm(&inc, v, "_INCLUDEDIR", NULL);
    stralloc_nul(&inc);
    stralloc_catm(&lib, v, "_LIBDIR", NULL);
    stralloc_nul(&lib);

    if(format_linkdir_fn) {
      stralloc ref;

      stralloc_init(&ref);
      stralloc_catm(&ref, "$(", lib.s, ")", NULL);
      stralloc_nul(&ref);
      format_linkdir_fn(ref.s, &dir);
      stralloc_free(&ref);
    }

    stralloc_nul(&dir);

    if(tool == TOOL_BATCH) {
      buffer_putm_internal(
          b, "IF DEFINED ", v, " SET DEFS=%DEFS% -D", macro, "=1 & SET LIBS=%LIBS% %", v, "%", newline, NULL);
      buffer_putm_internal(b, "IF DEFINED ", inc.s, " SET CPPFLAGS=%CPPFLAGS% -I%", inc.s, "%", newline, NULL);
      buffer_putm_internal(b, "IF DEFINED ", lib.s, " SET LDFLAGS=%LDFLAGS% ", dir.s, newline, NULL);
    } else if(tool == TOOL_SHELL) {
      buffer_putm_internal(
          b, "if [ -n \"$", v, "\" ]; then DEFS=\"$DEFS -D", macro, "=1\"; LIBS=\"$LIBS $", v, "\"; fi", newline, NULL);
      buffer_putm_internal(
          b, "if [ -n \"$", inc.s, "\" ]; then CPPFLAGS=\"$CPPFLAGS -I$", inc.s, "\"; fi", newline, NULL);
      buffer_putm_internal(b, "if [ -n \"$", lib.s, "\" ]; then LDFLAGS=\"$LDFLAGS ", dir.s, "\"; fi", newline, NULL);
    } else if(bang) {
      buffer_putm_internal(b,
                           "!ifdef ",
                           v,
                           newline,
                           "DEFS = $(DEFS) -D",
                           macro,
                           "=1",
                           newline,
                           "LIBS = $(LIBS) $(",
                           v,
                           ")",
                           newline,
                           NULL);
      buffer_putm_internal(
          b, "!ifdef ", inc.s, newline, "CPPFLAGS = $(CPPFLAGS) -I$(", inc.s, ")", newline, "!endif", newline, NULL);
      buffer_putm_internal(b,
                           "!ifdef ",
                           lib.s,
                           newline,
                           "LDFLAGS = $(LDFLAGS) ",
                           dir.s,
                           newline,
                           "!endif",
                           newline,
                           "!endif",
                           newline,
                           NULL);
    } else {
      buffer_putm_internal(
          b, "ifneq ($(", v, "),)", newline, "DEFS += -D", macro, "=1", newline, "LIBS += $(", v, ")", newline, NULL);
      buffer_putm_internal(
          b, "ifneq ($(", inc.s, "),)", newline, "CPPFLAGS += -I$(", inc.s, ")", newline, "endif", newline, NULL);
      buffer_putm_internal(b,
                           "ifneq ($(",
                           lib.s,
                           "),)",
                           newline,
                           "LDFLAGS += ",
                           dir.s,
                           newline,
                           "endif",
                           newline,
                           "endif",
                           newline,
                           NULL);
    }

    stralloc_free(&inc);
    stralloc_free(&lib);
    stralloc_free(&dir);
    alloc_free(macro);
  }

  buffer_putnl(b, 0);
  buffer_flush(b);
}

/**
 * @brief      Output a variable that a value given to make takes precedence over
 *
 * The assignments of the variables overrule the command line of Borland make.
 */
void
output_default(buffer* b, build_tool_t tool, const char* name, const char* value) {
  int bang = str_start(tools.make, "borland") || str_start(tools.make, "bmake") || str_start(tools.make, "nmake") ||
             str_start(tools.make, "po");

  if(tool == TOOL_NINJA)
    return;

  if(tool == TOOL_BATCH)
    buffer_putm_internal(b, "IF NOT DEFINED ", name, " SET ", name, "=", value, newline, NULL);
  else if(tool == TOOL_SHELL)
    buffer_putm_internal(b, ": ${", name, ":=", value, "}", newline, NULL);
  else if(bang)
    buffer_putm_internal(b, "!ifndef ", name, newline, name, " = ", value, newline, "!endif", newline, NULL);
  else
    buffer_putm_internal(b, name, " ?= ", value, newline, NULL);

  buffer_flush(b);
}
