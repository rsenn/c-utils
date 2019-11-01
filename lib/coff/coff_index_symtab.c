#include "../coff.h"

coff_symtab_entry*
coff_index_symtab(void* coff, int index) {
  ssize_t i = index;
  range symtab = coff_symbol_table(coff);

  if(i >= 0 && i < (ssize_t)range_size(&symtab))
    return range_index(&symtab, i);

  return NULL;
}
