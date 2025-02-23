#include "../cpp_internal.h"

static int
visit(const void* key, size_t key_len, const void* value, size_t value_len, void* user_data) {
  const char* n = key;
  cpp_macro* m = (void*)value;
  buffer* b = user_data;

  if(m->num_args & (MACRO_FLAG_OBJECTLIKE | MACRO_FLAG_VARIADIC))
    buffer_putm_internal(b, (m->num_args & MACRO_FLAG_OBJECTLIKE) ? "objectlike " : (m->num_args & MACRO_FLAG_VARIADIC) ? "variadic   " : "default    ", 0);

  if(m->num_args & MACRO_ARGCOUNT_MASK) {
    buffer_putulong(b, (m->num_args & MACRO_ARGCOUNT_MASK));
    buffer_puts(b, " args ");
  }

  buffer_putm_internal(b, n, "=", m->str_contents_buf, 0);

  buffer_putnlflush(b);
}

void
cpp_dump(cpp* pp, buffer* out) {
  MAP_VISIT_ALL(pp->macros, visit, out);
}
