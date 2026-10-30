#include "../ini.h"
#include "../str.h"

const char*
ini_get(ini_section_t* ini, const char* key) {
  return MAP_GET(ini->map, (char*)key, str_len(key) + 1);
}
