#include "../ini.h"
#include "../str.h"

void
ini_set(ini_section_t* ini, const char* key, const char* value) {
  MAP_INSERT2(ini->map, (char*)key, str_len(key) + 1, (char*)value, str_len(value) + 1);
}
