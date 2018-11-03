#include "../pe.h"
#include "../uint32.h"

uint32
pe_offset2rva(uint8* base, int64 off) {
  int i, n;
  pe_section_header* sections = pe_header_sections(base, &n);

  if((uint32)off < sections[0].pointer_to_raw_data)
    return off;

  for(i = 0; i < n; i++) {
    uint32 start, size;
    uint32_unpack(&sections[i].pointer_to_raw_data, &start);
    uint32_unpack(&sections[i].size_of_raw_data, &size);

    if((uint32)off >= start && (uint32)off < start + size)
      return (uint32)off - start + sections[i].virtual_address;
  }
  return 0;
}
