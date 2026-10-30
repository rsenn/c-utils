#include "../ini.h"

void
ini_set_sa(ini_section_t* ini, stralloc* key, stralloc* value) {
  stralloc_nul(key);
  stralloc_nul(value);
  ini_set(ini, key->s, value->s);
}
