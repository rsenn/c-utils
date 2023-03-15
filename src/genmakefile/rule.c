#include "rule.h"
#include "path.h"
#include "is.h"
#include <string.h>

MAP_T rules;

/**
 * Find or create rule
 */
target*
rule_get(const char* name) {
  static int rule_serial;
  target* ret = NULL;
  MAP_PAIR_T t = NULL;
  size_t len = str_len(name);
  if(!MAP_SEARCH(rules, name, len + 1, &t)) {
    target tgt;
    byte_zero(&tgt, sizeof(struct target_s));
    tgt.name = str_ndup(name, len);
    tgt.serial = ++rule_serial;
    tgt.type = -1;
    set_init(&tgt.output, 0);
    set_adds(&tgt.output, name);
    set_init(&tgt.prereq, 0);
    strlist_init(&tgt.cmds, ' ');
    MAP_INSERT(rules, name, len + 1, &tgt, ((sizeof(struct target_s) + 3) / 4) * 4);
    (MAP_SEARCH(rules, name, len + 1, &t));
    // ret = MAP_ITER_VALUE(t);

#ifdef DEBUG_OUTPUT_
    if(t) {
      buffer_putm_internal(buffer_2, "Created rule '", ((target*)MAP_ITER_VALUE(t))->name, "'\n", NULL);
      buffer_flush(buffer_2);
    }
#endif
  }
  if(t)
    ret = MAP_ITER_VALUE(t);
  return ret;
}

/**
 * @brief rule_get_sa
 * @param name
 * @return
 */
target*
rule_get_sa(stralloc* name) {
  stralloc_nul(name);
  return rule_get(name->s);
}

/**
 * @brief rule_find
 * @param needle
 * @return
 */
target*
rule_find(const char* needle) {
  MAP_PAIR_T t;
  MAP_FOREACH(rules, t) {
    const char* name = MAP_ITER_KEY(t);
    if(str_equal(name, needle))
      return MAP_ITER_VALUE(t);
    if(str_equal(path_basename((char*)name), path_basename((char*)needle)))
      return MAP_ITER_VALUE(t);
    /*
        if(t->next == rules->list_tuple)
          break; */
  }
  return 0;
}

void
rule_rename(target* rule, const char* name, char pathsep) {
  size_t len;
  stralloc out;
  stralloc_init(&out);
  stralloc_catset(&out, &rule->output, " ");
  len = byte_rchr(out.s, out.len, pathsep);
  if(out.len > len)
    out.len = len + 1;
  else
    out.len = 0;
  set_adds(&rule->output, name);
  free((char*)rule->name);
  rule->name = str_ndup(out.s, out.len);
}

/**
 * @brief rule_find_sa
 * @param name
 * @return
 */
target*
rule_find_sa(stralloc* name) {
  stralloc_nul(name);
  return rule_find(name->s);
}

/**
 * @brief rule_find_b
 * @param x
 * @param n
 * @return
 */
target*
rule_find_b(const char* x, size_t n) {
  target* r;
  stralloc sa;
  stralloc_init(&sa);
  sa.s = (char*)x;
  sa.len = n;
  r = rule_find_sa(&sa);
  return r;
}

target*
rule_find_lib(const char* name, size_t namelen, const char* libext, const char* slibext) {
  target* t;
  stralloc pattern;
  size_t dot;
  stralloc_init(&pattern);
  stralloc_cats(&pattern, "lib");
  stralloc_catb(&pattern, name, namelen);
  dot = pattern.len;
  stralloc_cats(&pattern, libext);
  if((t = rule_find_sa(&pattern)))
    return t;
  pattern.len = dot;
  stralloc_cats(&pattern, slibext);
  if((t = rule_find_sa(&pattern)))
    return t;
  return 0;
}

/**
 * @brief rule_match
 * @param rule           Target rule
 * @param pattern        Preqreq
 * wildcard pattern
 * @return               1 if match
 */
int
rule_match(target* rule, const char* pattern) {
  int ret = 0;
  const char* s;
  size_t n;
  set_iterator_t it;
  stralloc sa;
  stralloc_init(&sa);
  set_foreach(&rule->prereq, it, s, n) {
    stralloc_copyb(&sa, s, n);
    if(str_equal(pattern, path_wildcard(&sa, "%"))) {
      ret = 1;
      break;
    }
  }
  stralloc_free(&sa);
  return ret;
}

/**
 * @brief rule_command_subst  Get rule
 * command with substitutions
 * @param rule
 * @param out
 * @param prereq
 * @param plen
 */
void
rule_command_subst(target* rule,
                   stralloc* out,
                   const char* prereq,
                   size_t plen,
                   bool shell,
                   bool batch,
                   const char quote_args[],
                   char pathsep_args,
                   const char* make_sep_inline) {
  size_t i;
  stralloc* in = &rule->recipe;
  for(i = 0; i < in->len; ++i) {
    const char* p = &in->s[i];
    if((shell || batch) && i + 4 <= in->len && *p == '$' && p[1] == '(') {
      size_t vlen;
      stralloc_catc(out, shell ? '$' : '%');
      i += 2;
      vlen = byte_chr(&in->s[i], in->len - i, ')');
      stralloc_catb(out, &in->s[i], vlen);
      if(batch)
        stralloc_catc(out, '%');
      i += vlen;
      continue;
    }
    if(i + 2 <= in->len && *p == '$' && str_chr("@^<|", p[1]) < 4) {
      switch(p[1]) {
        case '@': {
          size_t p = out->len;
          stralloc_catq(out, rule->name, str_len(rule->name), quote_args);
          byte_replace(&out->s[p], out->len - p, pathsep_args == '/' ? '\\' : '/', pathsep_args);
          break;
        }
        case '^': {
          stralloc_catq(out, prereq, plen, quote_args);
          break;
        }
        case '|': {
          stralloc_subst(out, prereq, plen, " ", make_sep_inline ? make_sep_inline : "\n ");
          break;
        }
        case '<': {
          size_t n = byte_chr(prereq, plen, ' ');
          stralloc_catq(out, prereq, n, quote_args);
          break;
        }
      }
      i++;
    } else {
      if(!stralloc_append(out, p))
        break;

      if(*p == '\n')
        stralloc_catc(out, '\t');
    }
    stralloc_nul(out);
  }
}

/**
 * @brief rule_command
 * @param rule
 * @param out
 */
void
rule_command(target* rule,
             stralloc* out,
             bool shell,
             bool batch,
             const char quote_args[],
             char pathsep_args,
             const char* make_sep_inline,
             const char* maketool) {
  size_t len;
  const char* pfx = 0;
  char *s, from = pathsep_args == '/' ? '\\' : '/';
  set_iterator_t it;
  strlist prereq;
  strlist_init(&prereq, ' ');
  if(stralloc_contains(&rule->recipe, "-+$^")) {
    // pfx = "-+";
  }
  set_foreach(&rule->prereq, it, s, len) {
    if(pfx) {
      strlist_push(&prereq, pfx);
      stralloc_catb(&prereq.sa, s, len);
    } else {
      strlist_pushb_unique(&prereq, s, len);
    }
  }
  // stralloc_copy(&prereq.sa, &rule->prereq.sa);
  stralloc_replacec(&prereq.sa, from, pathsep_args);
  if(0 /* make_begin_inline == NULL  && rule->recipe ==  &commands.lib*/) {
    char* x;
    size_t n = 0;
    range r;
    r.start = stralloc_begin(&prereq.sa);
    r.end = stralloc_end(&prereq.sa);
    for(; r.start < r.end;) {
      for(x = r.start;;) {
        n = strlist_skip(&prereq, x);
        if(n == 0 || x + n - r.start > 512)
          break;
        x += n;
      }
      if(out->len)
        stralloc_cats(out, "\n\t");

      if(pfx && byte_equal(r.start, str_len(pfx), pfx)) {
        r.start += 2;
      }
      n = x - r.start;
      if(n > 0 && r.start[n - 1] == ' ')
        n--;
      rule_command_subst(rule, out, r.start, n, shell, batch, quote_args, pathsep_args, make_sep_inline);
      if(r.start + n < r.end && r.start[n] == ' ')
        n++;
      r.start += n;
    }
  } else if(!str_start(maketool, "g") && !(rule->name[0] == '.' && strchr(&rule->name[1], '.') && prereq.sa.len == 0)) {
    rule_command_subst(rule, out, prereq.sa.s, prereq.sa.len, shell, batch, quote_args, pathsep_args, make_sep_inline);
  } else {
    const char *p, *end;
    for(p = out->s, end = out->s + out->len; p < end;) {
      size_t linelen = byte_chr(p, end - p, '\n');
      if(p[linelen] != '\0')
        ++linelen;

      stralloc_catc(out, '\t');
      stralloc_catb(out, p, linelen);
      p += linelen;
    }
  }
  stralloc_free(&prereq.sa);
}

/**
 * @brief rule_add_dep
 * @param t
 * @param other
 * @return
 */
int
rule_add_dep(target* t, target* other) {
  target** ptr;
  if((ptr = array_find(&t->deps, sizeof(target*), &other)) == NULL) {
    array_catb(&t->deps, &other, sizeof(other));
    array_foreach_t(&other->deps, ptr) { rule_add_dep(t, *ptr); }
    return 1;
  }
  return 0;
}

/**
 * @brief rule_add_deps
 * @param t
 * @param deps
 */
void
rule_add_deps(target* t, const strlist* deps) {
  const char* x;
  size_t n;
  strlist_foreach(deps, x, n) {
    target* other;
    if(str_len(t->name) == n && !str_diffn(t->name, x, n))
      continue;
    if((other = rule_find_b(x, n))) {
      rule_add_dep(t, other);
    }
  }
}

/**
 * @}
 */
/**
 * @brief rule_dep_list_recursive Lists
 * all dependencies of a target
 * @param l Output target names
 * @param t Target
 */
static uint32 rule_dep_serial;
void
rule_dep_list_recursive(target* t, set_t* s, int depth, strlist* hier) {
  target** ptr;
  t->serial = rule_dep_serial;
  array_foreach_t(&t->deps, ptr) {
    const char* name = (*ptr)->name;
    if(t->serial == (*ptr)->serial)
      continue;
    if(!strlist_contains(hier, name)) {
      strlist_push(hier, name);
      rule_dep_list_recursive(*ptr, s, depth + 1, hier);
      strlist_pop(hier);
      if(depth >= 0) {
        if(set_adds(s, name)) {
        }
      }
    }
  }
}

/**
 * @brief rule_dep_list
 * @param l
 * @param t
 */
void
rule_dep_list(target* t, set_t* s) {
  strlist hier;
  strlist_init(&hier, '\0');
  strlist_push(&hier, t->name);
  set_clear(s);
  --rule_dep_serial;
  rule_dep_list_recursive(t, s, 0, &hier);
  strlist_free(&hier);
}

/**
 * @brief indirect_dep_list  List all
 * indirect deps of a target
 * @param l                  Output
 * target names
 * @param t                  Target
 */
void
rule_deps_indirect(target* t, set_t* s) {
  target** ptr;
  strlist hier;
  strlist_init(&hier, '\0');
  strlist_push(&hier, t->name);
  set_adds(s, t->name);
  array_foreach_t(&t->deps, ptr) {
    if(*ptr)
      rule_dep_list_recursive(*ptr, s, 0, &hier);
  }
  // set_deletes(s, t->name);
  strlist_free(&hier);
}

void
rule_prereq(target* t, set_t* s) {
  set_iterator_t it;
  const char* x;
  size_t n;
  set_foreach(&t->prereq, it, x, n) { set_add(s, x, n); }
}

void
rule_prereq_recursive(target* t, set_t* s) {
  set_iterator_t it;
  const char* x;
  size_t n;
  set_foreach(&t->prereq, it, x, n) {
    target* rule;
    if((rule = rule_find_b(x, n))) {
      if(rule != t)
        rule_prereq_recursive(rule, s);
    } else {
      set_add(s, x, n);
    }
  }
}

/**
 * @brief rule_dump
 * @param rule
 */
void
rule_dump(target* rule) {
#ifdef DEBUG_OUTPUT
  buffer_putm_internal(buffer_2, "\n  ", YELLOW256, "NAME   ", NC, " ", NULL);
  buffer_puts(buffer_2, rule->name);
  buffer_puts(buffer_2, "\n  " PURPLE256 "RECIPE " NC " ");
  buffer_putsa_escaped(buffer_2, &rule->recipe, fmt_escapecharshell);
  buffer_puts(buffer_2, "\n  " BLUE256 "OUTPUT " NC " ");
  buffer_putset(buffer_2, &rule->output, " ", 1);
  buffer_puts(buffer_2, "\n  " GREEN256 "PREREQ " NC "\n  \t");
  buffer_putset(buffer_2, &rule->prereq, "\n  \t", 4);
  if(array_length(&rule->deps, sizeof(target*))) {
    buffer_puts(buffer_2, "\n  DEPS   " NC "\n  \t");
    print_rule_deps(buffer_2, rule);
  }
  buffer_putnlflush(buffer_2);
  return;
#endif

  buffer_putm_internal(buffer_2, "Rule '", rule->name, 0, "'\n", NULL);
  if(set_size(&rule->prereq)) {
    buffer_puts(buffer_2, "\n  prereq: ");
    buffer_putset(buffer_2, &rule->prereq, " ", 1);
  }
  if(set_size(&rule->output)) {
    buffer_puts(buffer_2, "\n  output: ");
    buffer_putset(buffer_2, &rule->output, " ", 1);
  }
  if(rule->recipe.len) {
    stralloc_nul(&rule->recipe);
    buffer_putm_internal(buffer_2, "\n  recipe: ", rule->recipe.s, "\n", NULL);
  }
  if(array_length(&rule->deps, sizeof(target*))) {
    target** t;
    buffer_puts(buffer_2, "\n  deps:");
    array_foreach_t(&rule->deps, t) {
      buffer_putspace(buffer_2);
      buffer_puts(buffer_2, (*t)->name);
    }
    buffer_putc(buffer_2, '\n');
  }
  buffer_putnlflush(buffer_2);
}

bool
rule_is_compile(target* rule) {
  size_t n;
  const char* x;
  if(stralloc_contains(&rule->recipe, " -c "))
    return true;
  if((x = set_at_n(&rule->prereq, 0, &n)))
    if(is_source_b(x, n))
      return true;
  if(is_object(rule->name))
    return true;
  return false;
}

bool
rule_is_lib(target* rule) {
  size_t n;
  const char* x;
  set_iterator_t it;
  if(str_end(rule->name, exts.lib))
    return true;
  set_foreach(&rule->prereq, it, x, n) {
    if(!is_object_b(x, n))
      return false;
  }
  return set_size(&rule->prereq) > 0;
}

bool
rule_is_link(target* rule) {
  size_t n;
  const char* x;
  set_iterator_t it;
  set_foreach(&rule->prereq, it, x, n) {
    if(!is_object_b(x, n) && !is_lib_b(x, n))
      return false;
  }
  return set_size(&rule->prereq) > 0;
}
