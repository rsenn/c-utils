#include "ini.h"
#include "map.h"
#include "lib/str.h"
#include "lib/alloc.h"
#include "lib/stralloc.h"

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
  MAP_ITER_T t;

  while(ini) {
    buffer_putc(b, '[');
    buffer_putsa(b, &ini->name);
    buffer_put(b, "]\r\n", 3);

    MAP_FOREACH(ini->map, t) {
      buffer_put(b, t->key, t->key_len - 1);
      buffer_putc(b, '=');
      buffer_put(b, t->vals.val_chars, t->data_len - 1);
      buffer_puts(b, "\r\n");
    }
    if(ini->next)
      buffer_puts(b, "\r\n");

    ini = ini->next;
  }
  buffer_flush(b);
}