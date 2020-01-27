#include "ini.h"
#include "lib/str.h"

void
ini_set(ini_section_t* ini, const char* key, const char* value) {
  MAP_INSERT(ini->map, key, str_len(key) + 1, value, str_len(value) + 1);
}

void
ini_write(buffer* b, ini_section_t* ini) {
  MAP_ITER_T t;
  buffer_putc(b, '[');
  buffer_putsa(b, &ini->name);
  buffer_putc(b, ']');
  buffer_putnlflush(b);

  MAP_FOREACH(ini->map, t) {}
}