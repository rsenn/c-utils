#include "../ini.h"

ini_section_t*
ini_section(ini_section_t* ini, const char* name) {
  do

    if(stralloc_case_equals(&ini->name, name))
      return ini;

  while((ini = ini->next));

  return 0;
}
