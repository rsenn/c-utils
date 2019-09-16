#include "../coff.h"

const char*
coff_symbol_name(void* coff, coff_symtab_entry* sym) {
  if(sym->e.zeroes == 0) {
    const char* strtab = coff_get_strtab(coff, NULL);

    return &strtab[sym->e.offset];
  }
  return sym->e.name;
}
