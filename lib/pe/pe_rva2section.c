#include "../pe.h"

int
pe_rva2section(void* base, uint32 rva) {
  uint16 i, n;
  pe_section_header* sections = pe_header_sections(base, &n);

  if(rva < sections[0].virtual_address)
    return -1;

  for(i = 0; i < n; i++) {
    uint32 start = sections[i].virtual_address;
    uint32 end = start + sections[i].size_of_raw_data;

    if(rva >= start && rva < end)
      return i;
  }
  return -1;
}
