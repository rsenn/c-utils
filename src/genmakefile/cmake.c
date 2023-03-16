#define MAP_USE_HMAP 1
#include "../../lib/stralloc.h"
#include "../../lib/path.h"
#include "../../genmakefile.h"
#include "cmake.h"
#include "../../lib/strarray.h"
#include "../../lib/map.h"
#include "../../lib/set.h"
#include <stdbool.h>
#include "is.h"

void
output_cmake_var(buffer* b, const char* name, const strlist* list) {
  buffer_putm_internal(b, "set(", name, " ", NULL);
  buffer_putsa(b, &list->sa);
  buffer_puts(b, ")");
  buffer_putnlflush(b);
}

void
append_cmake_var(buffer* b, const char* name, const strlist* list) {
  buffer_putm_internal(b, "set(", name, " ${", name, "} ", NULL);
  buffer_putsa(b, &list->sa);
  buffer_puts(b, ")");
  buffer_putnlflush(b);
}

void
output_cmake_cmd(buffer* b, const char* cmd, const strlist* list, char quote) {
  if(strlist_count(list)) {
    char needle[2] = {quote, '\\'};
    const char* s;
    size_t n;
    buffer_putm_internal(b, cmd, "(\n", NULL);
    strlist_foreach(list, s, n) {
      size_t i;
      buffer_puts(b, "  ");
      if(quote)
        buffer_PUTC(b, quote);

      for(i = 0; i < n; i++) {
        if(byte_chr(needle, 2, s[i]) < 2)
          buffer_putc(b, '\\');
        buffer_putc(b, s[i]);
      }
      if(quote)
        buffer_PUTC(b, quote);
      buffer_puts(b, "\n");
    }
    buffer_puts(b, ")\n");
    buffer_putnlflush(b);
  }
}

void
output_cmake_set(buffer* b, const char* cmd, const set_t* list, char quote) {
  if(set_size(list)) {
    char needle[2] = {quote, '\\'};
    const char* s;
    size_t n;
    set_iterator_t it;
    buffer_putm_internal(b, cmd, "(\n", NULL);
    set_foreach(list, it, s, n) {
      size_t i;
      buffer_puts(b, "  ");
      if(quote)
        buffer_PUTC(b, quote);

      for(i = 0; i < n; i++) {
        if(byte_chr(needle, 2, s[i]) < 2)
          buffer_putc(b, '\\');
        buffer_putc(b, s[i]);
      }
      if(quote)
        buffer_PUTC(b, quote);
      buffer_puts(b, "\n");
    }
    buffer_puts(b, ")\n");
    buffer_putnlflush(b);
  }
}
void
output_cmake_subst(stralloc* str, const char* varname) {
  const char* value;
  if((value = var_get(varname))) {
    stralloc ref;
    stralloc_init(&ref);
    stralloc_catm_internal(&ref, "${", varname, "}", NULL);
    stralloc_nul(&ref);
    stralloc_replaces(str, value, ref.s);
  }
}

void
output_cmake_subst_path(const char* path, const char* varname, stralloc* out) {
  const char* value;
  stralloc_zero(out);
  path_normalize(path, out);
  output_cmake_subst(out, varname);
}

/*
void
output_cmake_libs(buffer* b) {
  strlist* libs = var_list("LIBS");
  const char* s;
  size_t n;
  buffer_puts(b, "\nlink_libraries(");
  strlist_foreach(libs, s, n) {
    if(n < 2 || !byte_equal(s, 2, "-l"))
      continue;

    buffer_puts(b, "\n  ");
    buffer_put(b, s + 2, byte_finds(s + 2, n - 2, "$("));
  }
  buffer_puts(b, "\n)");
  buffer_putnlflush(b);
}
*/
void
output_cmake_rule(buffer* b, target* rule) {
  bool compile = stralloc_contains(&rule->recipe, " -c ");
  set_iterator_t it;
  const char* x;
  size_t n;

  set_foreach(&rule->output, it, x, n) {

    if(!compile) {
      bool lib = rule_is_lib(rule);
      bool link = !(rule_is_compile(rule) || lib);
      size_t pos = 0;
      set_t deps, libs, srcs;

      set_init(&libs, 0);
      set_init(&deps, 0);
      set_init(&srcs, 0);
      buffer_puts(b, lib ? "add_library(\n  " : "add_executable(\n  ");
      if(lib || link) {
        pos = byte_rchr(x, n, '/');
        if(pos < n)
          pos++;
        if(lib) {
          if(n - pos >= 3 && byte_equal(x + pos, 3, "lib"))
            pos += 3;
          if(byte_ends(x + pos, n - pos, exts.lib))
            n -= str_len(exts.lib);
          rule_prereq_recursive(rule, &deps);
        }
      }
      /*   if(link) {
           set_filter_out(&rule->prereq, &deps, is_source_b);
           set_filter(&rule->prereq, &srcs, is_source_b);
         } else */
      {

        const char* s;
        size_t n;
        set_iterator_t it;
        set_foreach(&rule->prereq, it, s, n) {
          target* compile;

          if(n >= 2 && byte_equal(s, 2, "./")) {
            s += 2;
            n -= 2;
          }

          if(is_object_b(s, n)) {

            if((compile = rule_find_b(s, n))) {
              rule_prereq(compile, &srcs);
//            set_cat(&srcs, &compile->prereq);
#ifdef DEBUG_OUTPUT
              buffer_puts(buffer_2, "compile rule '");
              buffer_puts(buffer_2, compile->name);
              buffer_puts(buffer_2, "' : ");
              buffer_putset(buffer_2, &compile->prereq, " ", 1);
              buffer_putnlflush(buffer_2);
#endif
            } else {
              buffer_puts(buffer_2, "rule '");
              buffer_put(buffer_2, s, n);
              buffer_puts(buffer_2, "' not found");
              buffer_putnlflush(buffer_2);
              exit(1);
            }
          } else if(is_lib_b(s, n)) {
            set_add(&libs, s, n);
          } else if(is_source_b(s, n)) {
            set_add(&srcs, s, n);

          } else {
            set_add(&deps, s, n);
          }
        }
      }

      if(n - pos == 0 /* || 0 == set_size(lib ? &deps : &srcs)*/)
        continue;

      buffer_put(b, x + pos, n - pos);
      buffer_puts(b, "\n  ");

      if(lib && is_lib(rule->name))
        buffer_puts(b, "STATIC\n  ");

      buffer_putset(b, lib ? &deps : &srcs, "\n  ", 3);
      buffer_puts(b, "\n)");
      buffer_putnlflush(b);

      if(set_size(&libs)) {
        buffer_puts(b, "target_link_libraries(");
        buffer_put(b, x + pos, n - pos);
        buffer_putc(b, ' ');
        buffer_putset(b, &libs, " ", 1);
        buffer_puts(b, ")");
        buffer_putnlflush(b);
      }

#ifdef DEBUG_OUTPUT
      {
        strarray v;

        strarray_init(&v);
        set_tostrarray(&srcs, &v);
        strarray_sort(&v, 0);

        buffer_puts(buffer_2, "deps:\n  ");
        buffer_putset(buffer_2, &deps, "\n  ", 3);
        buffer_putnlflush(buffer_2);
        buffer_puts(buffer_2, "srcs:\n  ");
        buffer_putstra(buffer_2, &v, "\n  ", 3);
        buffer_putnlflush(buffer_2);
        strarray_free(&v);
      }
#endif
    }
  }
}

void
output_cmake_rules(buffer* b, MAP_T rules) {
  MAP_PAIR_T t;
  MAP_FOREACH(rules, t) {
    // target* rule = MAP_ITER_VALUE(t);
    const char* name = MAP_ITER_KEY(t);
    target* rule = MAP_ITER_VALUE(t);

#ifdef DEBUG_OUTPUT_
    buffer_putm_internal(buffer_2, "Outputting cmake ", NULL);
    rule_dump(rule);
#endif
    if(!cmd_libs && str_end(name, ".a"))
      continue;
    /* if(str_end(name, ".o"))
       continue;*/
    if(str_equal(name, "all") || str_equal(name, "clean"))
      continue;

#ifdef DEBUG_OUTPUT_
    buffer_puts(buffer_2, "Outputting rule '");
    buffer_puts(buffer_2, MAP_ITER_KEY(t));
    buffer_putc(buffer_2, '\'');
    buffer_putnlflush(buffer_2);
#endif
    output_cmake_rule(b, rule);
  }
}

void
output_cmake_project(buffer* b, MAP_T* rules, MAP_T* vars, const strlist* include_dirs, const strlist* link_dirs) {
  MAP_PAIR_T t;
  set_t libraries;
  buffer_putm_internal(b, "project(", project_name, " ", sources_iscplusplus() ? "CXX" : "C", ")", NULL);
  buffer_putnlflush(b);
  buffer_putnlflush(b);

  MAP_FOREACH(*vars, t) {
    const char* name = MAP_ITER_KEY(t);

    buffer_puts(buffer_2, "var: ");
    buffer_puts(buffer_2, name);
    buffer_putnlflush(buffer_2);
  }
  set_init(&libraries, 0);

  MAP_FOREACH(*rules, t) {
    target* rule = MAP_ITER_VALUE(t);

    set_filter(&rule->prereq, &libraries, is_lib_b);
  }
  //  return 0;
  const char* s;
  size_t n;
  set_iterator_t it;
  stralloc sa;

  set_foreach(&libraries, it, s, n) {
    stralloc_init(&sa);
    // path_dirname_b(s, n, &sa);

#ifdef DEBUG_OUTPUT
    buffer_puts(buffer_2, "libdir: ");
    buffer_put(buffer_2, s, path_dirlen_b(s, n));
    buffer_putnlflush(buffer_2);
#endif
#ifdef DEBUG_OUTPUT
    buffer_puts(buffer_2, "lib: ");
    buffer_put(buffer_2, s, n);
    buffer_putnlflush(buffer_2);
#endif
    stralloc_free(&sa);
  }

  output_cmake_subst(&include_dirs->sa, "CMAKE_CURRENT_SOURCE_DIR");
  output_cmake_subst(&include_dirs->sa, "CMAKE_CURRENT_BINARY_DIR");
  output_cmake_subst(&link_dirs->sa, "CMAKE_CURRENT_BINARY_DIR");

  if(var_isset("CFLAGS"))
    output_cmake_var(b, "CMAKE_C_FLAGS", &var_list("CFLAGS", ' ')->value);
  if(var_isset("CXXFLAGS"))
    output_cmake_var(b, "CMAKE_CXX_FLAGS", &var_list("CXXFLAGS", ' ')->value);

  if(var_isset("LDFLAGS"))
    output_cmake_var(b, "CMAKE_EXE_LINKER_FLAGS", &var_list("LDFLAGS", ' ')->value);

  /*  if(var_isset("DEFS")) {
      buffer_puts(b, "add_definitions(");
      buffer_putsa(b, &var_list("DEFS")->sa);
      buffer_puts(b, ")");
      buffer_putnlflush(b);
    }
  */
  buffer_putnlflush(b);
  output_cmake_cmd(b, "add_definitions", &var_list("DEFS", ' ')->value, 0);
  output_cmake_set(b, "link_libraries", &link_libraries, 0);
  // output_cmake_libs(b);
  output_cmake_cmd(b, "include_directories", include_dirs, 0);

  output_cmake_cmd(b, "link_directories", link_dirs, 0);

  output_cmake_rules(b, *rules);
}
