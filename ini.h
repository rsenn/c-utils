#ifndef _INI_H
#define _INI_H

#include "lib/byte.h"
#include "lib/stralloc.h"
#include "lib/strarray.h"
#include "lib/buffer.h"

#define MAP_USE_HASHMAP 1
#include "lib/map.h"

typedef struct ini_section {
  struct ini_section* next;
  stralloc name;
  MAP_T map;
} ini_section_t;

ini_section_t* ini_newb(ini_section_t** ptr,
                        const char* name,
                        size_t namelen);
ini_section_t* ini_new(ini_section_t** ptr, const char* name);
ini_section_t* ini_section(ini_section_t* ini, const char* name);
strarray ini_keys(ini_section_t* ini);
const char* ini_get(ini_section_t* ini, const char* key);
void ini_set(ini_section_t* ini, const char* key, const char* value);
void ini_set_long(ini_section_t* ini, const char* key, long value);
void ini_set_sa(ini_section_t* ini, stralloc* key, stralloc* value);
void ini_write(buffer* b, ini_section_t* ini, int utf16);
void ini_read(buffer* b, ini_section_t** ini);

static void
ini_init(ini_section_t* ini) {
  byte_zero(ini, sizeof(ini_section_t));
}

#endif /* defined _INI_H */
