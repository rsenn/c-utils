#include "../pe.h"

int64
pe_rva2offset(void* base, uint32 rva) {
  int i, n;
  pe_section_header* sections = pe_header_sections(base, &n);
 
  if(rva < sections[0].virtual_address)
    return rva;

  for(i = 0; i < n; i++) {
    uint32 start = sections[i].virtual_address;
    uint32 end = start + sections[i].size_of_raw_data;

    if(rva >= start && rva < end)
      return rva - start + sections[i].pointer_to_raw_data;
  }
  return -1;
}
