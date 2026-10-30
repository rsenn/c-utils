#include "../ini.h"
#include "../str.h"

ini_section_t*
ini_new(ini_section_t** ptr, const char* name) {
  return ini_newb(ptr, name, str_len(name));
}
