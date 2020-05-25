#ifndef _INI_H
#define _INI_H

#include "map.h"
#include "lib/byte.h"
#include "lib/stralloc.h"
#include "lib/buffer.h"

typedef struct ini_section {
  struct ini_section* next;
  stralloc name;
  MAP_T map;
} ini_section_t;

ini_section_t* ini_new(ini_section_t** ptr, const char* name);
static void
ini_init(ini_section_t* ini) {
  byte_zero(ini, sizeof(ini_section_t));
}
void ini_set(ini_section_t* ini, const char* key, const char* value);
void ini_set_long(ini_section_t* ini, const char* key, long value);
void ini_set_sa(ini_section_t* ini, stralloc* key, stralloc* value);
void ini_write(buffer* b, ini_section_t* ini);

#endif /* defined _INI_H */
