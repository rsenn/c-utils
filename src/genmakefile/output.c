#include "../../genmakefile.h"
#include "../../debug.h"
#include "ansi.h"

/**
 * @brief      Extract variables from buffer
 *
 * @param[in]  x     Buffer
 * @param[in]  len   Length
 * @param      s     Output set
 */
static void
extract_vars(const char* x, size_t len, set_t* out) {
  set_clear(out);

  for(size_t i = 0; i < len; ++i) {
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

  set_init(&refvars, 0);

  if(MAP_SEARCH(*vars, name, str_len(name) + 1, &t)) {
    var = MAP_ITER_VALUE(t);

    if(var->serial == serial)
      return;

    stralloc_init(&v);

    if(var->value.sa.len) {
      stralloc_copys(&v, MAP_ITER_KEY(t));

      if(tool == TOOL_NINJA)
        stralloc_lower(&v);

      stralloc_nul(&v);
      set_clear(&refvars);

      if(!infile) {
        extract_vars(var->value.sa.s, var->value.sa.len, &refvars);

        set_foreach(&refvars, it, ref, len) {
#ifdef DEBUG_OUTPUT
          buffer_putm_internal(buffer_2, "Var ", name, " ref: ", ref, NULL);
          buffer_putnlflush(buffer_2);
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
          var_subst(&u, &v, "$", "", 1);
        } else if(tool == TOOL_BATCH) {
          stralloc_zero(&v);
          var_subst(&u, &v, "%", "%", 1);
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

  strlist_foreach_s(varnames, name) { output_var(b, vars, name, serial, tool); }

  buffer_putnl(b, 1);
}

/**
 * @brief      Output a make rule
 *
 * @param      b                Output buffer
 * @param      rule             The rule
 * @param[in]  tool             Build tool
 * @param[in]  quote_args
 * @param[in]  psa              Path separator for arguments
 * @param[in]  psm              Path separator in Makefile
 * @param[in]  make_sep_inline  Make separator inline
 */
void
output_make_rule(buffer* b, target* rule, build_tool_t tool, const char quote_args[], char psa, char psm, const char* make_sep_inline) {
  const char* x;
  stralloc output, sa, name;
  size_t n, num_prereqs = set_size(&rule->prereq);

  debug_nl = "\n";

  stralloc_init(&output);
  stralloc_init(&sa);
  stralloc_init(&name);

  stralloc_copys(&name, rule->name);

#ifdef DEBUG_OUTPUT
  buffer_puts(buffer_2, "RULE\n  " YELLOW256 "name" NC ": ");
  buffer_puts(buffer_2, rule->name);
  buffer_puts(buffer_2, "\n  " PURPLE256 "recipe" NC ": ");
  buffer_putsa(buffer_2, &rule->recipe);
  buffer_puts(buffer_2, "\n  " BLUE256 "output" NC ":\n\t");
  buffer_putset(buffer_2, &rule->output, "\n\t", 2);
  buffer_puts(buffer_2, "\n  " GREEN256 "prereq" NC ":\n\t");
  buffer_putset(buffer_2, &rule->prereq, "\n\t", 2);

  if(array_length(&rule->deps, sizeof(target*))) {
    buffer_puts(buffer_2, "\n  deps" NC ": ");
    print_rule_deps(buffer_2, rule);
  }

  buffer_putnlflush(buffer_2);
#endif

  if(rule->phony || (num_prereqs == 0 && str_diffn(rule->name, dirs.work.sa.s, dirs.work.sa.len) && !rule->name[str_chr(rule->name, psm)] && str_end(rule->name, ":"))) {
    buffer_putm_internal(b, ".PHONY: ", rule->name, newline, NULL);
  }

  stralloc_zero(&sa);
  stralloc_catset(&sa, &rule->output, " \\\n");

#ifdef DEBUG_OUTPUT_
  buffer_puts(buffer_2, "RULE OUTPUT: ");
  buffer_putsa_escaped(buffer_2, &sa, &fmt_escapecharcontrol);
  buffer_putnlflush(buffer_2);
#endif

  strlist_foreach(&rule->vars, x, n) {
    buffer_putsa(b, &sa);
    buffer_puts(b, ": ");
    buffer_put(b, x, n);
    buffer_puts(b, "\n\n");
  }

  stralloc_zero(&output);
  stralloc_nul(&output);

  if(str_contains(rule->name, "%")) {
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

  stralloc_catc(&output, ':');

  if(num_prereqs) {
    const char* str;
    size_t len;
    bucket_t* it;
    int i = 0;

    stralloc_cats(&output, num_prereqs > 1 ? " \\\n\t" : " ");

    set_foreach_ordered(&rule->prereq, it, str, len) {
      if(stralloc_endb(&output, str, len))
        continue;

      if(i)
        stralloc_cats(&output, num_prereqs > 1 ? " \\\n\t" : " ");

      stralloc_catb(&output, str, len);
      i++;
    }
  }

  if(rule->recipe.s) {
    stralloc cmd;

    stralloc_init(&cmd);

    /* XXX

    if(infile && (signed)rule->type >= 0)
      stralloc_copy(&cmd, &commands.v[rule->type]);
    else*/
    rule_command(rule, &cmd, tool == TOOL_SHELL, tool == TOOL_BATCH, quote_args, psa, make_sep_inline, tools.make);

    stralloc_remove_all(&cmd, "\0", 1);

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
    stralloc_replaces(&output, dirs.work.sa.s, "$(BUILDDIR)");
    // stralloc_replaces(&output, dirs.out.sa.s, "$(DISTDIR)");
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

  if(rule_is_compile(rule) || rule->recipe.s == commands.compile.s)
    rule_name = "cc";
  else if(rule_is_link(rule) || rule->recipe.s == commands.link.s)
    rule_name = "link";
  else if(rule_is_lib(rule) || stralloc_equal(&rule->recipe, &commands.lib))
    rule_name = "lib";

  if(rule_name) {
    stralloc path;

    stralloc_init(&path);
    set_at_sa(&rule->output, 0, &path);

    stralloc_replaces(&path, dirs.build.sa.s, "$objdir");

    buffer_puts(b, "build ");
    buffer_putsa(b, &path);
    buffer_puts(b, ": ");
    buffer_puts(b, rule_name);
    buffer_puts(b, " ");

    stralloc_zero(&path);

    path_relative_to_sa(&dirs.out.sa, &dirs.work.sa, &obj_dir);

    {
      const char* x;
      size_t n, i = 0;
      set_iterator_t it;
      stralloc tmp, outdir;

      stralloc_init(&tmp);
      stralloc_init(&outdir);
      path_concat_sa(&dirs.this.sa, &dirs.out.sa, &outdir);

      set_foreach(&rule->prereq, it, x, n) {
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
    stralloc_replaces(&path, dirs.build.sa.s, "$objdir");

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

  var_subst(cmd, &out, "$", "", 1);

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
 * @param[in]  quote_args       Quote arguments
 * @param[in]  psa              Path separator for arguments
 * @param[in]  psm              Path separator for Makefile
 * @param[in]  make_sep_inline  Make separator inline
 */
void
output_all_rules(buffer* b, build_tool_t tool, const char quote_args[], char psa, char psm, const char* make_sep_inline) {
  MAP_PAIR_T t;

  MAP_FOREACH(rules, t) {
    const char* name = MAP_ITER_KEY(t);
    target* rule = MAP_ITER_VALUE(t);

    if(!cmd_libs && str_end(name, ".a"))
      continue;

    if(rule->disabled)
      continue;

#ifdef DEBUG_OUTPUT
    buffer_puts(buffer_2, "Outputting rule '");
    buffer_puts(buffer_2, name);
    buffer_putc(buffer_2, '\'');
    buffer_putnlflush(buffer_2);
#endif

    if(tool == TOOL_NINJA)
      output_ninja_target(b, rule, psa);
    else
      output_make_rule(b, rule, tool, quote_args, psa, psm, make_sep_inline);
  }
}

/**
 * @brief      Output a script
 *
 * @param      b                Output buffer
 * @param      rule             The rule
 * @param[in]  tool             Build tool
 * @param[in]  quote_args       The quote arguments
 * @param[in]  psa              The psa
 * @param[in]  make_sep_inline  The make separator inline
 */
void
output_script(buffer* b, target* rule, build_tool_t tool, const char quote_args[], char psa, const char* make_sep_inline) {
  static uint32 serial;
  char* x;
  size_t n;
  set_iterator_t it;
  int flush = 0;

  if(rule == NULL) {
    flush = 1;
    ++serial;
    rule = rule_get("all");
    ++serial;
  }

  if(rule->serial == serial)
    return;

  if(!rule->name[str_chr(rule->name, '%')])

    if(rule->recipe.s != commands.compile.s)
      buffer_putm_internal(b, newline, tool == TOOL_BATCH ? "REM" : "#", " Rules for '", rule->name, "'", newline, NULL);

  set_foreach(&rule->prereq, it, x, n) {
    target* dep = rule_find_b(x, n);

    if(!dep || dep->serial == serial)
      continue;

    output_script(b, dep, tool, quote_args, psa, make_sep_inline);
  }

  if(array_length(&rule->objs, sizeof(target*))) {
    target** tptr;

    array_foreach_t(&rule->objs, tptr) {
      target* dep = *tptr;

      if(dep == 0 || dep->serial == serial)
        continue;

      output_script(b, dep, tool, quote_args, psa, make_sep_inline);
    }
  }

  if(rule->recipe.len) {
    stralloc cmd;

    stralloc_init(&cmd);

    rule_command(rule, &cmd, tool == TOOL_SHELL, tool == TOOL_BATCH, quote_args, psa, make_sep_inline, tools.make);
    buffer_putsa(b, &cmd);

    if(tool == TOOL_BATCH)
      buffer_puts(b, " || GOTO FAIL");

    stralloc_free(&cmd);
  }

  if(str_equal(rule->name, "all")) {
    if(tool == TOOL_BATCH)
      buffer_putm_internal(
          b, newline, ":SUCCESS", newline, "ECHO Done.", newline, "GOTO QUIT", newline, newline, ":FAIL", newline, "ECHO Fail.", newline, newline, ":QUIT", newline, 0);
  }

  buffer_putnl(b, flush);
  rule->serial = serial;
}
