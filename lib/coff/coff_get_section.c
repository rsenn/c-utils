#include "../coff.h"

coff_section_header*
coff_get_section(void* coff, uint16 index) {
  coff_file_header* fhdr = coff_header_file(coff);

  if(index > 0 && index <= uint16_get(&fhdr->number_of_sections))
    return &coff_header_sections(coff, NULL)[index - 1];

  return NULL;
}

