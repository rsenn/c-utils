#define MAP_USE_HMAP 1
#include "genmakefile.h"
#include "cmake.h"
#include "lib/map.h"
#include "lib/set.h"
#include <stdbool.h>

void
output_cmake_var(buffer* b, const char* name, const strlist* list) {

  const char* s;
  size_t n;
  buffer_putm_internal(b, "set(", name, " ", 0);

  buffer_putsa(b, &list->sa);
  buffer_puts(b, ")");
  buffer_putnlflush(b);
}

void
append_cmake_var(buffer* b, const char* name, const strlist* list) {
  const char* s;
  size_t n;
  buffer_putm_internal(b, "set(", name, " ${", name, "} ", 0);
  buffer_putsa(b, &list->sa);
  buffer_puts(b, ")");
  buffer_putnlflush(b);
}

void
output_cmake_cmd(buffer* b, const char* cmd, const strlist* list) {
  if(strlist_count(list)) {
    const char* s;
    size_t n;
    buffer_putm_internal(b, cmd, "(\n", 0);
    strlist_foreach(list, s, n) {
      size_t i;
      buffer_puts(b, "  \"");

      for(i = 0; i < n; i++) {
        if(byte_chr("\"\\", 2, s[i]) < 2)
          buffer_putc(b, '\\');
        buffer_putc(b, s[i]);
      }
      buffer_puts(b, "\"\n");
    }
    buffer_puts(b, ")\n");
    buffer_putnlflush(b);
  }
}

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
      set_t deps;

      set_init(&deps, 0);

      buffer_puts(b, lib ? "add_library(" : "add_executable(");
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
      buffer_put(b, x + pos, n - pos);
      buffer_putc(b, ' ');

      if(lib && is_lib(rule->name))
        buffer_puts(b, "STATIC ");

      buffer_putset(b, lib ? &deps : &rule->prereq, " ", 1);
      buffer_puts(b, ")");
      buffer_putnlflush(b);
    }

#ifdef DEBUG_OUTPUT
    buffer_puts(buffer_2, "rule ");
    buffer_put(buffer_2, x, n);
    buffer_puts(buffer_2, ": ");
    buffer_putset(buffer_2, &rule->prereq, " ", 1);
    /*    buffer_puts(buffer_2, ", ");
        buffer_putsa(buffer_2, &rule->recipe);*/
    buffer_putnlflush(buffer_2);
#endif
  }
}

void
output_cmake_rules(buffer* b, MAP_T rules) {
  MAP_PAIR_T t;

  MAP_FOREACH(rules, t) {
    target* rule = MAP_ITER_VALUE(t);
    const char* name = MAP_ITER_KEY(t);

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
    output_cmake_rule(b, MAP_ITER_VALUE(t));
  }
}

void
output_cmake_project(buffer* b, MAP_T* rules, MAP_T* vars, const strlist* include_dirs, const strlist* link_dirs) {
  const char* s;
  size_t n;
  buffer_putm_internal(b, "project(", project_name, " ", sources_iscplusplus() ? "CXX" : "C", ")", 0);
  buffer_putnlflush(b);
  buffer_putnlflush(b);

  if(var_isset("CFLAGS"))
    output_cmake_var(b, "CMAKE_C_FLAGS", var_list("CFLAGS"));
  if(var_isset("CXXFLAGS"))
    output_cmake_var(b, "CMAKE_CXX_FLAGS", var_list("CXXFLAGS"));

  if(var_isset("LDFLAGS"))
    output_cmake_var(b, "CMAKE_EXE_LINKER_FLAGS", var_list("LDFLAGS"));

  /*  if(var_isset("DEFS")) {
      buffer_puts(b, "add_definitions(");
      buffer_putsa(b, &var_list("DEFS")->sa);
      buffer_puts(b, ")");
      buffer_putnlflush(b);
    }
  */
  buffer_putnlflush(b);
  output_cmake_cmd(b, "add_definitions", var_list("DEFS"));
  // output_cmake_cmd(b, "link_libraries", var_list("LIBS"));
  output_cmake_libs(b);
  output_cmake_cmd(b, "include_directories", include_dirs);
  output_cmake_cmd(b, "link_directories", link_dirs);

  output_cmake_rules(b, *rules);
}
