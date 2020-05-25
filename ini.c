#include "ini.h"
#include "map.h"
#include "lib/str.h"
#include "lib/fmt.h"
#include "lib/alloc.h"
#include "lib/stralloc.h"
#include "lib/buffer.h"

void
ini_set(ini_section_t* ini, const char* key, const char* value) {
  MAP_INSERT(ini->map, (char*)key, str_len(key) + 1, (char*)value, str_len(value) + 1);
}

void
ini_set_sa(ini_section_t* ini, stralloc* key, stralloc* value) {
  stralloc_nul(key);
  stralloc_nul(value);
  ini_set(ini, key->s, value->s);
}

void
ini_set_long(ini_section_t* ini, const char* key, long value) {
  char buf[FMT_LONG + 1];
  size_t n;

  n = fmt_long(buf, value);
  buf[n] = '\0';
  return ini_set(ini, key, buf);
}

ini_section_t*
ini_new(ini_section_t** ptr, const char* name) {
  ini_section_t* ini = alloc(sizeof(ini_section_t));

  ini_init(ini);

  MAP_NEW(ini->map);

  stralloc_init(&ini->name);
  stralloc_copys(&ini->name, name);

  ini->next = NULL;
  *ptr = ini;

  return ini;
}

void
ini_write(buffer* b, ini_section_t* ini) {
  MAP_PAIR_T t;

  while(ini) {
    buffer_putc(b, '[');
    buffer_putsa(b, &ini->name);
    buffer_put(b, "]\r\n", 3);

    MAP_FOREACH(ini->map, t) {
      buffer_put(b, MAP_KEY(t), str_len(MAP_KEY(t)));
      buffer_putc(b, '=');
      buffer_put(b, MAP_VALUE(t), str_len(MAP_VALUE(t)));
      buffer_puts(b, "\r\n");
    }

    ini = ini->next;
  }
  buffer_flush(b);
}
