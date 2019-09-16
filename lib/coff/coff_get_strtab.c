#include "../coff.h"

const char*
coff_get_strtab(void* coff, uint32* szptr) {
  range syms = coff_symbol_table(coff);
  uint32 size = uint32_get(syms.end);
  if(szptr)
    *szptr = size;
  return syms.end;
}
