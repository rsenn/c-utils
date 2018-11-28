#include "../coff.h"

coff_symtab_entry*
coff_index_symtab(void* coff, int index) {
  range symtab = coff_symbol_table(coff);

  if(index >= 0 && index < range_size(&symtab))
    return range_index(&symtab, index);

  return NULL;
}