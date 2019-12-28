#include "../coff.h"
#include "../range.h"

range
coff_symbol_table(void* coff) {
  coff_file_header* fhdr = coff_header_file(coff);
  range r;

  r.start = (char*)coff + uint32_get(&fhdr->pointer_to_symbol_table);
  r.elem_size = fhdr->machine == COFF_FILE_MACHINE_MICROCHIP_V2 ? 20 : 18;
  r.end = r.start + r.elem_size * uint32_get(&fhdr->number_of_symbols);

  return r;
}
