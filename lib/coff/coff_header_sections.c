#include "../coff.h"

coff_section_header*
coff_header_sections(void* obj, uint16* nsections) {
  coff_file_header* coff = coff_header_file(obj);
  coff_opt_header* opthdr = coff_header_opt(obj);
  uint16 optsize = uint16_get(&coff->size_of_optional_header);

  if(nsections) {
    uint16 num = uint16_get(&coff->number_of_sections);
    *nsections = num;
  }
  return (coff_section_header*)((char*)opthdr + optsize);
}
