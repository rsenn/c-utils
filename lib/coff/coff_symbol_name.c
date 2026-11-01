#include "../coff.h"

/* The name of a symbol: in the entry itself if it fits in 8 bytes (NOT NUL
 * terminated then), else in the string table. */
const char*
coff_symbol_name(void* coff, coff_symtab_entry* sym) {
  if(sym->e.zeroes == 0) {
    uint32 size = 0;
    const char* strtab = coff_get_strtab(coff, &size);

    /* the offset comes from the file: a bad one used to read outside the table */
    if(sym->e.offset >= size)
      return "<bad string table offset>";
    return &strtab[sym->e.offset];
  }
  return sym->e.name;
}
