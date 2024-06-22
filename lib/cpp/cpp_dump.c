#include "../cpp_internal.h"

static int
visit(const void* key, size_t key_len, const void* value, size_t value_len, void* user_data) {
  const char* n = key;
  cpp_macro* m = (void*)value;
  buffer* b = user_data;

  buffer_putm_internal(b, "key '", n, "' str: '", m->str_contents_buf, "'", 0);
  buffer_putnlflush(b);
}

void
cpp_dump(cpp* pp, buffer* out) {
  MAP_VISIT_ALL(pp->macros, visit, out);
}
