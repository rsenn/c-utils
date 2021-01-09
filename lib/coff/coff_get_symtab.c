#include "../coff.h"

coff_symtab_entry*
coff_get_symtab(void* coff, uint32* num) {
  coff_file_header* fhdr = coff_header_file(coff);

  if(num) {
    *num = uint32_get(&fhdr->number_of_symbols);
  }
  return (coff_symtab_entry*)((char*)coff +
                              uint32_get(&fhdr->pointer_to_symbol_table));
}
