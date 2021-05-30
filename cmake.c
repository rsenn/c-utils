#define MAP_USE_HMAP 1
#include "genmakefile.h"
#include "cmake.h"
#include "lib/map.h"
#include "lib/set.h"

void
output_cmake_var(buffer* b, const char* name) {
  strlist* list;

  if((list = var_list(name))) {
    const char* s;
    size_t n;
    buffer_putm_internal(b, "set(", name, 0);

    strlist_foreach(list, s, n) {
      buffer_puts(b, "\n  ");
      buffer_put(b, s, n);
    }
    buffer_puts(b, ")");
    buffer_putnlflush(b);
  }
}

void
output_cmake_project(buffer* b, MAP_T* _rules, MAP_T* vars, const strlist* include_dirs) {
  const char* s;
  size_t n;
  buffer_putm_internal(b, "project(", project_name, " ", "C", ")", 0);
  buffer_putnlflush(b);
  buffer_putnlflush(b);

  output_cmake_var(b, "CFLAGS");
  output_cmake_var(b, "CPPFLAGS");
  output_cmake_var(b, "CXXFLAGS");

  buffer_puts(b, "include_directories(");

  strlist_foreach(include_dirs, s, n) {
    buffer_puts(b, "\n  \"");
    buffer_put(b, s, n);
    buffer_puts(b, "\"");
  }
  buffer_puts(b, ")");
  buffer_putnlflush(b);
}
