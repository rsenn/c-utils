#include "../coff.h"

range
coff_line_numbers(void* coff, coff_section_header* shdr) {
  range r;

  r.start = (char*)coff + uint32_get(&shdr->pointer_to_linenumbers);
  r.elem_size = 6;
  r.end = r.start + r.elem_size * uint16_get(&shdr->number_of_linenumbers);

  return r;
}


