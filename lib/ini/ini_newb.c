#include "../ini.h"
#include "../alloc.h"

ini_section_t*
ini_newb(ini_section_t** ptr, const char* name, size_t namelen) {
  ini_section_t* ini = alloc(sizeof(ini_section_t));

  ini_init(ini);

  MAP_NEW(ini->map);

  stralloc_init(&ini->name);
  stralloc_copyb(&ini->name, name, namelen);

  ini->next = NULL;
  *ptr = ini;

  return ini;
}
