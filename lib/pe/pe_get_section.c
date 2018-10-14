#include "../pe.h"
#include "../str.h"

pe_section_header*
pe_get_section(void* base, const char* name) {
  int i, n;
  pe_section_header* sections = pe_header_sections(base, &n);

  for(i = 0; i < n; i++) {
    if(str_equal(name, sections[i].name))
      return &sections[i];
  }

  return 0;
}

