#include "../ini.h"

/* The section with the name (not case sensitive) in the list that starts at
 * ini, or NULL; also for a file that held no section at all (ini is NULL). */
ini_section_t*
ini_section(ini_section_t* ini, const char* name) {
  for(; ini; ini = ini->next)
    if(stralloc_case_equals(&ini->name, name))
      return ini;

  return 0;
}
