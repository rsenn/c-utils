// #define NO_UINT16_MACROS 1
#include "../uint16.h"
#include "../pe.h"

pe_section_header*
pe_header_sections(void* pe, uint16* nsections) {
  pe_coff_header* coff = pe_header_coff(pe);
  unsigned char* opthdr = (unsigned char*)&coff[1];
  uint16 optsize = uint16_get(&coff->size_of_optional_header);

  if(nsections) {
    uint16 num = uint16_get(&coff->number_of_sections);
    *nsections = num;
  }
  return (pe_section_header*)(opthdr + optsize);
}
