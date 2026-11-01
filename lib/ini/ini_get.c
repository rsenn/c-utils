#include "../ini.h"
#include "../str.h"

const char*
ini_get(ini_section_t* ini, const char* key) {
  /* a file that was not there gives no section, and no value */
  if(ini == NULL || key == NULL)
    return NULL;

  return MAP_GET(ini->map, (char*)key, str_len(key) + 1);
}
