#include "../pe.h"

int64
pe_rva2offset(void* base, uint32 rva) {
  int i;
  pe_section_header* sections;

  if((i = pe_rva2section(base, rva)) == -1)
    return -1;

  sections = pe_header_sections(base, NULL);

  return rva - sections[i].virtual_address +
         sections[i].pointer_to_raw_data;
}
